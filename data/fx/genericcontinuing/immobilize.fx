#############################################################
## HEADER
#############################################################

FxInfo

Flags    InheritAlpha

ClampMaxScale 3.5 3.5 3.5

Lifespan 9000

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LLegR
		Bhvr	:Speed_DeBuff_Feet.bhvr
		part	:Speed_DeBuff_Ring_Feet.part
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	:Speed_DeBuff_Feet.bhvr
		part	:Speed_DeBuff_Ring_Feet.part
	End

	Event
		Type	Local
		At	FootR
		part	:Slow_02.part
		part	:Slow_02b.part
		POther	LLEgL
	End

	Event
		Type	Local
		At	FootL
		part	:Slow_02.part
		part	:Slow_02b.part
		POther	LLegR
	End

	Event
		Type	Local
		At	Root
		part	:Speed_DeBuff_Ring_Feet.part
	End
End

#############################################################

End