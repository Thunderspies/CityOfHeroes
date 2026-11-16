#############################################################
## StrengthHands2.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	Left
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Lifespan 45
	End
End
##################Sound#####################################
Condition

	On	Time
	Time	0

	Event
		EName	Sound
		Type	Local
		At	hips
		Sound radcharge 60 60 .35
	End
	
End
Condition

	On	Time
	Time	45

	Event
		EName	Sound
		Type	Local
		At	hips
		Sound Enforced_Morale_Cast_01 50 50 .85
	End

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	Left
#		Type	Local
#		At	WepR
#		Bhvr	behaviors\GenericParticleFade.bhvr
#		Part1	:Strength.part
#		Part2	:StrengthSmall.part
#		Part3	:AbilityDots.part
#		Part4	:AbilitySparklingDots.part
#	End
#
#	Event
#		EName	Right
#		Type	Local
#		At	WepL
#		Bhvr	behaviors\GenericParticleFade.bhvr
#		Part1	:Strength.part
#		Part2	:StrengthSmall.part
#		Part3	:AbilityDots.part
#		Part4	:AbilitySparklingDots.part
#	End
#End

#############################################################

End