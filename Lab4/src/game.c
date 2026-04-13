#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "lab4.h"

extern Node      *g_root;
extern EditStack  g_undo;
extern EditStack  g_redo;
extern Hash       g_index;

/* ----------------------------------------------------------------
 * TODO 31  run_diagnosis
 *
 * Walk the decision tree iteratively (no recursion) using a
 * FrameStack.  At each question node ask the user yes/no and push
 * the appropriate child.  At each solution leaf display the fix and
 * ask whether it solved the problem.
 *
 * If the fix did not help, enter the learning phase:
 *   - Ask the user what would actually fix the problem.
 *   - Ask for a yes/no question that distinguishes their problem
 *     from the solution just shown.
 *   - Ask which answer applies to their problem.
 *   - Create a new question node and a new solution node, wire them
 *     correctly, graft them into the tree, record an Edit for
 *     undo/redo, and index the new question with canonicalize/h_put.
 *
 * Edge case: if parent is NULL the root itself must be replaced.
 * ---------------------------------------------------------------- */
void run_diagnosis(void) {
    clear();
    attron(COLOR_PAIR(5) | A_BOLD);
    mvprintw(0, 0, "%-80s", " Tech Support Diagnosis");
    attroff(COLOR_PAIR(5) | A_BOLD);

    mvprintw(2, 2, "I'll help diagnose your tech problem.");
    mvprintw(3, 2, "Answer each question with y or n.");
    mvprintw(4, 2, "Press any key to start...");
    refresh();
    getch();

    Node *Parent = NULL;
    int wasYes = -1;
    FrameStack stack;
    fs_init(&stack);
    fs_push(&stack, g_root, -1);
    while(!fs_empty(&stack)){
        clear();
        Frame Currentframe;
        Currentframe= fs_pop(&stack);
        if (Currentframe.node->isQuestion==1){
            int answer = get_yes_no(6, 2, Currentframe.node->text);
            Parent = Currentframe.node;
            wasYes = answer;
            if (answer == 1) fs_push(&stack, Currentframe.node->yes, 1);
            else if (answer == 0) fs_push(&stack, Currentframe.node->no, 0);
        }
        // It was a leaf node (solution node)
        else {
            // We ask the user if the solution satisfied their problem
            mvprintw(6, 2, "Try this: %s", Currentframe.node->text);
            int result = get_yes_no(8, 2, "Did this solve your problem? (y/n): ");
            if (result == 1){
                mvprintw(10, 2, "Awsome!");
                break;
            }
            else{
                char *newFix = strdup(get_input(10, 2, "What would actually fix it?"));
                char *differentiatorQ = strdup(get_input(12, 2, "Yes/no question to tell them apart?"));
                int ansForNew = get_yes_no(14, 2, "For YOUR fix, is the answer yes?");

                // create New node
                Node *newQuestion = create_question_node(differentiatorQ);
                Node *newLeaf = create_solution_node(newFix);
                if (ansForNew == 1){
                    newQuestion->yes = newLeaf;
                    newQuestion->no = Currentframe.node;
                }
                else {
                    newQuestion->no = newLeaf;
                    newQuestion->yes = Currentframe.node;
                }
                if (Parent == NULL){
                    g_root = newQuestion;
                }
                else if (wasYes == 1){
                    Parent->yes = newQuestion;
                }
                else{
                    Parent->no = newQuestion;
                }
                // The Edit is a recipet of this surgery
                Edit e;
                e.type = EDIT_INSERT_SPLIT;
                e.parent = Parent;
                e.wasYesChild = wasYes;
                e.oldLeaf = Currentframe.node;
                e.newQuestion = newQuestion;
                e.newLeaf = newLeaf;
                es_push(&g_undo, e); // put the reciept on the undo pile
                es_clear(&g_redo); // throw away the entire redo pile
                char *key = canonicalize(differentiatorQ);
                h_put(&g_index, key, count_nodes(g_root) - 1);
                free(key);

                free(newFix);
                free(differentiatorQ);
                break;
            }


        }



    }
    fs_free(&stack);

}

/* ----------------------------------------------------------------
 * TODO 32  undo_last_edit
 * Return 1 on success, 0 if the undo stack is empty.
 * ---------------------------------------------------------------- */
int undo_last_edit(void) {
    if (es_empty(&g_undo) == 1) return 0;
    Edit edit = es_pop(&g_undo);
    if (edit.parent == NULL) g_root = edit.oldLeaf;
    else if (edit.wasYesChild == 1){
        edit.parent->yes = edit.oldLeaf;
    }
    else {
        edit.parent->no = edit.oldLeaf;
    }
    es_push(&g_redo, edit);
    return 1;
}

/* ----------------------------------------------------------------
 * TODO 33  redo_last_edit
 * Return 1 on success, 0 if the redo stack is empty.
 * ---------------------------------------------------------------- */
int redo_last_edit(void) {
    if (es_empty(&g_redo) == 1) return 0;
    Edit edit = es_pop(&g_redo);
    if (edit.parent == NULL) g_root = edit.newQuestion;
    else if (edit.wasYesChild==1){
        edit.parent->yes = edit.newQuestion;
    }
    else {
        edit.parent->no = edit.newQuestion;
    }
    es_push(&g_undo, edit);
    return 1;
}
