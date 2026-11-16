#########################################################
##	Ice_Wall
#########################################################

FxInfo
Input  
	Inpname	Origin
End

Input  
	Inpname	Root
End

Input  
	Inpname	Hips
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Start 
		At	Hips
		part1	:woomer.part
		#part2	:DeepFreezeColdMistFlat.part
		#Part3	:DeepFreezeIceChunks.part	
			
	End

End

End