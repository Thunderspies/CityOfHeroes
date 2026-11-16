#########################################################
##	
##

FxInfo

#LifeSpan	100



Flags    InheritAlpha

###########################################################


Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	HandL
		Sound Quills1b 70 70 .76
	End
End

###########################################################
Condition
	On 	Time
	Time 	5



	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	V_Quill_WepL
			
	End

	
End

Condition
	On 	Time
	Time 	15

	Event

		EName	GasSpurts
		Type	Local
		At	HandL
		
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		lifeSpan	16
	End

End	

##################################
Condition
	On 	Time
	Time 	5



	Event
		EName	Spike2
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	V_Quill_WepR
			
	End

	
End

Condition
	On 	Time
	Time 	9

	Event
		EName	Spike
		Type	Local
		At	Chest
		Part2	:MustardGasSpecksHitA.part
		Part1	:MustardGasHitA.part
		
		LifeSpan	7	
	End

End

Condition
	On 	Time
	Time 	15

	Event

		EName	GasSpurtsb
		Type	Local
		At	HandR
				
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		lifeSpan	16
	End

End	

################################################################
##BackPoisonSpikes
################################################################

Condition
	On 	Time
	Time 	3
	Event
		EName	Spike02b
		Type	Local
		At	LarmL
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	V_Quill_LarmL
			
	End


	Event
		EName	Spike02b
		Type	Local
		At	LarmR
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	V_Quill_LarmR
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	SPadL
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	V_Quill_SPadL
			
	End


	Event
		EName	Spike02b
		Type	Local
		At	SpadR
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	V_Quill_SpadR
			
	End

End

Condition
	On 	Time
	Time 	3
	Event
		EName	Spike02b
		Type	Local
		At	waist
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	V_Quill_Waist
			
	End

End

Condition
	On 	Time
	Time 	5
	Event
		EName	Spike02b
		Type	Local
		At	chest
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	V_Quill_Chest01
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	chest
		
		Bhvr	Behaviors/PoisonSpikeScaleQuick.bhvr
		Geom	V_Quill_chest02
			
	End

End

#######################################################
##Legs
###############################################

Condition
	On 	Time
	Time 	2
	Event
		EName	Spike02b
		Type	Local
		At	UlegL
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	V_Quill_UlegL
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	UlegR
		
		Bhvr	Behaviors/PoisonSpikeScale2.bhvr
		Geom	V_Quill_UlegR
			
	End
End


##############################################################


Condition
	On 	Time
	Time 	3
	Event
		EName	Spike02b
		Type	Local
		At	LlegR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	V_Quill_LlegR
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegL
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	V_Quill_LlegL
	End

		
End


End				