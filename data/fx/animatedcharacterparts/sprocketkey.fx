#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End


Input  
	Inpname	Head
End

Input  
	InpName	LLEGL
End

Input  
	Inpname	LarmR
End


Input  
	Inpname	Hips
End


Input  
	Inpname	Neck
End

Flags    InheritAlpha

########################################################
## Key
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Chest
		Geom	DummyForKey
		
	
	End

	Event
		EName 	key
		Type	Local 
		At	Prime
		 AltPiv 1
		 Sound sprocket2_loop 30 30 .20
		Bhvr	behaviors/SprocketKey_spin.bhvr
		Geom	FX_SprocketKey
	
	End
End

########################################################
##Sparks
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	RArmSpark1
		Type	Posit 
		At	neck
		Part1	:SprocketNeckGlow.part
		Part2	:SprocketNeckEmberGlow.part
	End

	Event
		EName 	RArmSpark2
		Type	Posit 
		At	neck
		Part1	:SprocketNeckSpark.part
	
	End

	Event
		EName 	RArmSpark3
		Type	Local
		At	LarmR
		Part1	:SprocketRightArmSpark.part
	
	End

	Event
		EName 	RArmSpark4
		Type	Local 
		At	LarmR
		Part1	:SprocketArmGlow.part
	End



	Event
		EName 	RArmSpark5
		Type	Local
		At	LlegL
		Part1	:SprocketLegSpark.part
	
	End

	Event
		EName 	RArmSpark6
		Type	Local 
		At	LlegL
		Part1	:SprocketArmGlow.part
	End


End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		
		Geom	FX_SprocketKey
	
	End
	
	Event
		EName 	Key
		Type	Destroy
	
	End

	Event
		EName 	RArmSpark1
		Type	Destroy
	
	End

	Event
		EName 	RArmSpark2
		Type	Destroy
	
	End
	
	Event
		EName 	RArmSpark3
		Type	Destroy
	
	End

	Event
		EName 	RArmSpark4
		Type	Destroy
	
	End
	
	Event
		EName 	RArmSpark5
		Type	Destroy
	
	End
	
	Event
		EName 	RArmSpark6
		Type	Destroy
	
	End

End
##################################

