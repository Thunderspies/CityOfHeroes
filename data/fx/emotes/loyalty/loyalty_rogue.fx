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
		Part	:Circle_Orange.part
		Part	:Symbol_Blur_Rogue.part
		Part	:Symbol_Rogue.part
		Part	:Symbol_Additive_Rogue.part
	End


	Event
		EName	Ring
		Type	Local
		At	Origin
		ChildFX	:Stars_Orbiting_Rogue.fx
	End


End

#Condition
#	On	Time
#	Time	5
#
#	Event
#		Type	Start
#		At	Root
#		Part	:Symbol_Rogue.part
#		Part	:Symbol_Additive_Rogue.part
#	End
#End

#############################################################

End
