#############################################################
## HEADER
#############################################################

FxInfo

Flags    InheritAlpha

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandR
		Sound glowroom_loop 20 20 .14
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 280
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName	Body_Glow
		Type	Local
		At	LLegR
		part	:Speed_Buff_Ring.part
		Bhvr	:Speed_Buff_Feet.bhvr
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LLegL
		part	:Speed_Buff_Ring.part
		Bhvr	:Speed_Buff_Feet.bhvr
	End
End

#############################################################

End