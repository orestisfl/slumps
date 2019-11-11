from nodes import SolutionNode

def test_LLVM_Block_parser():

    text = '''
; LGTM
%0:i32 = var
%1:i32 = urem %0, 4:i32
pc %1 2:i32
%2:i32 = add 1:i32, %0
infer %2 (demandedBits=00000000000000000000000000000011)
result 4294967295:i32'''

    s = SolutionNode(text)
