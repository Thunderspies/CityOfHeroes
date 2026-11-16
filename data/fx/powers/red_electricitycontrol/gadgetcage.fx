#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Hips
End

########################################################


Condition
	On 	Time
	Time 	5

	Event
		
		Type	Posit
		At	Root
		Bhvr	behaviors/GadgetCageScale.bhvr
		Geom	FX_ElectricityCageA
		Sound energycore1_loop 60 60 .36
	
				
	End

	Event
		ENAME	TopElectricity
		Type	Posit
		At	Root
		Bhvr	behaviors/GadgetCageScale.bhvr
		Geom	FX_ElectricityCageTopsA
		Sound energycore2_loop 60 60 .55
	End


End


Condition
	On 	Time
	Time 	7

	Event
		
		Type	Posit
		At	Root
		Bhvr	behaviors/GadgetCageScale.bhvr
		Geom	FX_ElectricityCageB
		
				
	End
	
End

End
