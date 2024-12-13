//
// Created by root on 12/12/24.

#ifndef LOGIC_H
#define LOGIC_H


/**
 * basic:
 *
 *       true : always true
 *       false : always false
 *
 *       global variables:
 *       @global_memory : a map to store variables(use memory.key to access),visible to all
 *       @time: time passed since game begin (pause time excluded)
 *
 *
 *       unit only:
 *       @self : the unit itself
 *       @local_memory : a map to store variables(use memory.key to access),visible to the unit only
 *
 *
 * function:
 *
 *      if(bool) : return true if the condition is true
 *
 *      map only:
 *      once_if(bool) : return true only when the condition is first time true
 *
 *
 * operator:
 *       && : and
 *       || : or
 *       ! : not
 *       < : less than
 *       <= : less than or equal
 *       == : equal
 *       >= : greater than or equal
 *       > : greater than
 *       != : not equal
 *       + : add
 *       - : subtract
 *       * : multiply
 *       / : divide (float only)
 *       % : modulo
 */
class Logic
{
};


#endif //LOGIC_H
