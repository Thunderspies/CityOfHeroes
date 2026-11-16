#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
End

Flags    InheritAlpha

LifeSpan	30
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	Eyes
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
		LifeSpan	30
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistLeftBLUE.part
		sound	glow9b_loop 50 40 .21
		bhvr	behaviors/soundFade1.bhvr
		LifeSpan	30
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistRightBLUE.part
		LifeSpan	30
	End

End

End
##################################

