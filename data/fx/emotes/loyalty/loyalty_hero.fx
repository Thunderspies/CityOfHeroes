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
		Part	:Circle_Blue.part
		Part	:Symbol_Blur_Hero.part
		Part	:Symbol_Hero.part
		Part	:Symbol_Additive_Hero.part
	End

	Event
		EName	Ring
		Type	Local
		At	Origin
		ChildFX	:Stars_Orbiting.fx
	End
End

#Condition
#	On	Time
#	Time	5
#
#	Event
#		Type	Start
#		At	Root
#		Part	:Symbol_Hero.part
#		Part	:Symbol_Additive_Hero.part
#	End
#End

#############################################################

End
