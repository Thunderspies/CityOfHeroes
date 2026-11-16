#########################################################
##	TemperatureControl
########################################################

FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Hips
End

##########################
Condition
	On 	Time
	Time 	10

	Event
		EName 	Next
		Type	start 
		At	Root
		Bhvr 	Behaviors/Scale2.bhvr
		Geom	FX_TmpCtrlWave
		
	End


	Event
		EName 	Next
		Type	start 
		At	Root
		Part1   :snowswirlup.part
		Part2   :RingMistTempControl.part
		Part3   :FlatRingMistTempControl.part

	End
	
End

Condition
	On 	Time
	Time 	0


	Event
		Type	start 
		At	Hips
		#Bhvr 	powers/coldcontrol/icewave.bhvr
		Geom	FX_TempControlOrb
		Part1	:IceChunksTempControl.part
		Part2	:IceChunkTempControl.part
		Part3	:MistTempControl.part
		Part4	:FallingMistTempControl.part
	
	End

End





End