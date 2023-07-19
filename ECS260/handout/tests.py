from string import Template

primitive_datatypes = ['int', 'unsigned int', 'long', 'unsigned long', 'short', 'unsigned short', 'signed char', 'char',
                       'unsigned  char', 'long long', 'float', 'double', 'char*', 'int*']
format_specifiers = ['%d', '%u', '%ld', '%lu', '%hd', '%hu', '%c', '%c', '%c', '%lld', '%f', '%lf', '%s', '%d']
c = 1
arr_size = 3
access_index = [[[1], [3], [-1]], [[1, 1], [3, 1], [1, 3], [3, 3], [-1, 1], [1, -1], [-1, -1], [3, -1], [-1, 3]]]

template = '//Teammates: Sourin Chakrabarti, Ujwal Pratap Krishna Koluvakolanu Thyagarajan \n#include<stdio.h>\nint main() { $array_def $array_access return 0;}'


def get_array_def(dimension, datatype):
    array_def = ''
    if datatype == 'int*':
        array_def += 'int a=1, b=2, c=3;'
    array_def += datatype + ' arr'
    for _ in range(dimension):
        array_def += '[' + str(arr_size) + ']'

    array_def += '={'
    count = dimension
    if dimension == 2:
        count += 1
    for _ in range(count):
        if datatype == 'char*':
            array_def += '\"1\", \"2\", \"3\",'
        elif datatype == 'int*':
            array_def += '&a, &b, &c,'
        else:
            array_def += '1,2,3,'

    array_def = array_def[:-1] + '};'
    return array_def


def get_array_acc(dimension, format_specifier, access, datatype):
    array_acc = 'printf(\"arr'

    for d in range(dimension):
        array_acc += '[' + str(access[d - 1]) + ']'

    if datatype == 'int*':
        array_acc += ' = ' + format_specifier + '\", *arr'
    else:
        array_acc += ' = ' + format_specifier + '\", arr'

    for d in range(dimension):
        array_acc += '[' + str(access[d - 1]) + ']'

    array_acc += ');'
    return array_acc


for dimension in range(1, 3):
    for access in access_index[dimension - 1]:
        print('dimension: ', str(dimension) + ', access: ', access, ' starts at: ' + str(c))
        for datatype, format_specifier in zip(primitive_datatypes, format_specifiers):
            temp_obj = Template(template)
            file = open('tests/test' + str(c) + '.c', 'w')
            c += 1
            file.write(temp_obj.substitute(array_def=get_array_def(dimension, datatype),
                                           array_access=get_array_acc(dimension, format_specifier, access, datatype)))
