#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Circle_Red.part
		Part	:Circle_Red_Alpha.part
		Part	:Symbol_Blur_Villain.part
		Part	:Symbol_Villain.part
		Part	:Symbol_Additive_Villain.part
	End


	Event
		EName	Ring
		Type	Local
		At	Origin
		ChildFX	:Stars_Orbiting_Villain.fx
	End

End
#
#Condition
#	On	Time
#	Time	5
#
#	Event
#		Type	Start
#		At	Root
#		Part	:Symbol_Villain.part
#		Part	:Symbol_Additive_Villain.part
#	End
#End

#############################################################

End
