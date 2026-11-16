#############################################################
## Loyalty_Vigilante.fx
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
		Part	:Circle_DarkBlue.part
		Part	:Circle_DarkBlue_Alpha.part
		Part	:Symbol_Blur_Vigilante.part
		Part	:Symbol_Vigilante.part
		Part	:Symbol_Additive_Vigilante.part
	End


	Event
		EName	Ring
		Type	Local
		At	Origin
		ChildFX	:Stars_Orbiting_Vigilante.fx
	End
End

#Condition
#	On	Time
#	Time	5
#
#	Event
#		Type	Start
#		At	Root
#		Part	:Symbol_Vigilante.part
#		Part	:Symbol_Additive_Vigilante.part
#	End
#End

#############################################################

End
