#########################################################
## Generic Continuing Effect - Speed Debuff (All)
#########################################################

FxInfo

Flags    InheritAlpha

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

## DEBUFF FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Body_Glow
		Type	Local
		At	LArmR
		part	:Speed_DeBuff_Ring.part
		Bhvr	:Speed_DeBuff_Right.bhvr
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LArmL
		part	:Speed_DeBuff_Ring.part
		Bhvr	:Speed_DeBuff_Left.bhvr
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LLegR
		part	:Speed_DeBuff_Ring_Feet.part
		Bhvr	:Speed_DeBuff_Feet.bhvr
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LLegL
		part	:Speed_DeBuff_Ring_Feet.part
		Bhvr	:Speed_DeBuff_Feet.bhvr
	End
End

#########################################################

End