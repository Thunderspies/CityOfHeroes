#########################################################
##	forceBubble
########################################################
FxInfo

#####################
##Hands
############################################


Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Posit
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubbleLarge2.bhvr
		
		#Part1	:bubblesphereLarge.part
		Sound ForceField3_loop 100.0 100.0 .35
		#LifeSpan	30
		
	End

	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	:Hydra_Force_Field.bhvr
		Sound ForceField6_loop 100.0 100.0 .19
		
		#Geom	FX_Hydra_Force_Field
		
		#LifeSpan	50
				
	End

End


End
