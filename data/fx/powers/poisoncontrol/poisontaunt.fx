#########################################################
##	
##

FxInfo

LifeSpan	40

Input  
	InpName	Mystic
End

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End

Input  
	InpName	chest
End

Input  
	InpName	head
End

##################################
Condition
	On 	Time
	Time 	5



	Event
		EName	Spike
		Type	Local
		At	WepL
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	PoisonSpikeLeft

			
	End

End


#Condition
#	On 	Time
#	Time 	7
#
#	Event
#		EName	Spike01
#		Type	Local
#		At	Spike
#		
#		Bhvr	Behaviors/PoisonSpikeScale.bhvr
#		Geom	PoisonSpike03
#			
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	8.5
#	Event
#		EName	Spike02
#		Type	Local
#		At	Spike
#		
#		Bhvr	Behaviors/PoisonSpikeScaleQuick.bhvr
#		Geom	PoisonSpike05
#			
#	End
#End
#
#Condition
#	On 	Time
#	Time 	10
#	Event
#		EName	Spike02
#		Type	Local
#		At	Spike
#		
#		Bhvr	Behaviors/PoisonSpikeScale.bhvr
#		Geom	PoisonSpike04
#			
#	End
#
#End
#
Condition
	On 	Time
	Time 	15

	Event

		EName	GasSpurts
		Type	Local
		At	WepL
		
		
		Part1	:PoisonSpark.part
		
	End

End	


Condition
	On 	Time
	Time 	21

	Event

		EName	GasSpurts
		Type	destroy
			
	End

End
##################################
Condition
	On 	Time
	Time 	5



	Event
		EName	Spike2
		Type	Local
		At	WepR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	PoisonSpikeRight
			
	End

End


#Condition
#	On 	Time
#	Time 	7
#
#	Event
#		EName	Spike01b
#		Type	Local
#		At	Spike2
#		
#		Bhvr	Behaviors/PoisonSpikeScaleQuick.bhvr
#		Geom	PoisonSpike01
#			
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	8.5
#	Event
#		EName	Spike02
#		Type	Local
#		At	Spike2
#		
#		Bhvr	Behaviors/PoisonSpikeScale.bhvr
#		Geom	PoisonSpike06
#			
#	End
#End
#
#Condition
#	On 	Time
#	Time 	10
#	Event
#		EName	Spike02b
#		Type	Local
#		At	Spike2
#		
#		Bhvr	Behaviors/PoisonSpikeScaleQuick.bhvr
#		Geom	PoisonSpike02
#			
#	End
#
#End
#
Condition
	On 	Time
	Time 	15

	Event

		EName	GasSpurtsb
		Type	Local
		At	WepR
				
		Part1	:PoisonSpark.part
		
	End

End	


Condition
	On 	Time
	Time 	21

	Event

		EName	GasSpurtsb
		Type	destroy
			
	End

End

End				