import sys
    

def print_header():
    print(
"""#pragma once

#include <stdio.h>
#include <math.h>
""")
    
var_names = ['x', 'y', 'z', 'w']

def print_vectors():
    
    for i in range(2, 5): # 2 - 4
        values = ""
        print_args = ""
        floats_count = "[ "
        for v in range(0, i):
            values += '\t' + "float " + str(var_names[v]) + ';\n'
            print_args += "v->" + str(var_names[v]) + ", "
            floats_count += "%f "
        values = values.rstrip('\n')
        print_args = print_args.rstrip(', ')
        floats_count += "]"

        print(
format("""typedef struct Vector{i} {{
{values}
}} Vector{i};
       
void PrintVector{i}(Vector{i}* v, const char* name) {{
    printf("%s\\t: {floats_count}\\n", name, {print_args});
}}
""").format(i=i, values=values, floats_count=floats_count, print_args=print_args))

def print_vector_funcs():

    def generate_func(
        name: str,
        T: str,
        equation: str,
        strip: str,
        args_eq = None,
        value = None
    ):
        for i in range(2, 5):
            vec_type = "Vector" + str(i)
            
            args = ""
            if args_eq == None:
                args = vec_type + " v1, " + vec_type + " v2"
            else:
                args = args_eq(i)

            eq = "{ "
            for v in range(0, i):
                if value == None:
                    eq += equation(v)
                else:
                    eq += equation(v, value)

            eq = eq.rstrip(strip)
            eq += " }"

            return_type = vec_type if T == "vec" else T

            print(format(
"""{return_type} Vector{i}{name}({args}) {{
    {return_type} res = {eq};
    return res;
}}
""").format(return_type=return_type, i=i, name=name, args=args, eq=eq))
    # Add
    eq_add = lambda v: 'v1.' + var_names[v] + ' + ' + 'v2.' + var_names[v] + ', '
    generate_func("Add", "vec", eq_add, ", ")

    # Scale
    eq_scale = lambda v, value: 'v.' + var_names[v] + ' * ' + value + ', '
    args_scale = lambda i: 'Vector' + str(i) + ' v, float value'
    generate_func("Scale", "vec", eq_scale, ", ", args_scale, 'value')

    # Dot
    eq_dot = lambda v: "v1." + var_names[v] + ' * ' + "v2." + var_names[v] + ' + '
    generate_func("Dot", "float", eq_dot, "+ ")
    
if __name__ == '__main__':
    with open("sz_math.h", "w") as f:
        sys.stdout = f
        print_header()
        print_vectors()
        print_vector_funcs()