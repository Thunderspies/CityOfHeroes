#########################################################
##	
##

FxInfo

#LifeSpan	100


Flags    InheritAlpha
##################################
##################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	HandL
		Sound V_Quill1 70 70 .7
	End
End

##################################
##################################

Condition

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScale.bhvr
		Geom	V_Quill_WepL
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		Geom	V_QuillLeft
			
	End

End

Condition
	On 	Time
	Time 	4

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
	Time 	10

	Event

		EName	GasSpurts
		Type	Local
		At	HandL
		
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		#Part1	:PoisonSpark.part
		lifeSpan	16
	End

	
End	

##################################
Condition

	Event
		EName	Spike2
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScale.bhvr
		Geom	V_Quill_WepR
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		Geom	V_Quillright
			
	End
End



Condition
	On 	Time
	Time 	10

	Event

		EName	GasSpurtsb
		Type	Local
		At	HandR
		
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		#Part1	:PoisonSpark.part
		lifeSpan	16
	End

End	

################################################################
##BackPoisonSpikes
################################################################

Condition
	On 	Time
	Time 	0
	Event
		EName	Spike02b
		Type	Local
		At	waist
		
		Bhvr	Behaviors/QuillSpikeScale1.bhvr
		Geom	V_Quill_Waist
			
	End

End

Condition
	On 	Time
	Time 	0
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
	Time 	0
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
	Time 	0
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