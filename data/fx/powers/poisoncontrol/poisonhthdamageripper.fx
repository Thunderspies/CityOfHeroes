#########################################################
##	
##

FxInfo

#LifeSpan	100

Input  
	InpName	Mystic
End

Input  
	InpName	HandL
End

Input  
	InpName	HandR
End

Input  
	InpName	chest
End

Input  
	InpName	Waist
End

Input  
	InpName	head
End

Input  
	InpName	UlegL
End

Input  
	InpName	UlegR
End

Input  
	InpName	LlegL
End

Input  
	InpName	LlegR
End

Flags    InheritAlpha
###########################################################


Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	HandL
		Sound Quills1 70 70 .7
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
		Geom	Quills_WepL
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/PoisonSpikeScaleQuick.bhvr
		Geom	PoisonSpikeLeft
			
	End

End

Condition
	On 	Time
	Time 	7

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

		EName	GasSpurts
		Type	Local
		At	HandL
		
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		#Part1	:PoisonSpark.part
		lifeSpan	7
	End

	Event

		EName	GasSpurtsb
		Type	Local
		At	HandR
		
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		#Part1	:PoisonSpark.part
		lifeSpan	7
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
		Geom	Quills_WepR
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/PoisonSpikeScaleQuick.bhvr
		Geom	PoisonSpikeRight
			
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
		At	waist
		
		Bhvr	Behaviors/PoisonSpikeScale1.bhvr
		Geom	Quills_Waist
			
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
		Geom	Quills_Chest01
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	chest
		
		Bhvr	Behaviors/PoisonSpikeScaleQuick.bhvr
		Geom	Quills_chest02
			
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
		Geom	Quills_UlegL
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	UlegR
		
		Bhvr	Behaviors/PoisonSpikeScale2.bhvr
		Geom	Quills_UlegR
			
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
		Geom	Quills_LlegR
			
	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegL
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	Quills_LlegL
	End

		
End


End				