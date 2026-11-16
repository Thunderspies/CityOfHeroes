#########################################################
## Spines - Quill Burst
#########################################################

FxInfo

LifeSpan 87

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	Local
		At	HandL
		Sound Quills1 70 70 0.7
	End
End

Condition
	On 	Time
	Time 	46

	Event

		Type	Local
		At	HandL
		Sound Quills4 80 80 0.95
	End
End

## BODY SPINES #######################################################

##Condition
##	On 	Time
##	Time 	0
##
##	Event
##		Type	Start
##		At	Root
##		ChildFX :SpinesBodySpines.fx
##	End
##End

## HAND FX #######################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	GasSpurts
		Type	Local
		At	HandL
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		LifeSpan 14
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName	Spike
		Type	Local
		At	Chest
		Part2	:MustardGasSpecksHitA.part
		Part1	:MustardGasHitA.part
		LifeSpan 7
	End
End

Condition
	On 	Time
	Time 	15

	Event

		EName	GasSpurtsb
		Type	Local
		At	HandR
		Part1	:MustardGasHitBA.part
		Part2	:MustardGasSpecksHitBA.part
		LifeSpan 14
	End
End

## AOE #######################################################

Condition
	On 	Time
	Time 	48

	Event
		Ename	Spikes
		Type	start
		At	chest
		Anim	Fx_QuillBurst3
		Bhvr	Behaviors/QuilFade.bhvr
		LifeSpan 11
	End

	Event
		Ename	Spike1
		Type	local
		At	Spikes
		animpiv	Belt
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 14
	End

	Event
		Ename	Spike1
		Type	local
		At	Spikes
		animpiv	Waist
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike2
		Type	local
		At	Spikes
		animpiv	Chest
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 20
	End

	Event
		Ename	Spike3
		Type	local
		At	Spikes
		animpiv	Neck
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike4
		Type	local
		At	Spikes
		animpiv	Head
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 16
	End

	Event
		Ename	Spike5
		Type	local
		At	Spikes
		animpiv	UarmL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike6
		Type	local
		At	Spikes
		animpiv	UarmR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 14
	End

	Event
		Ename	Spike7
		Type	local
		At	Spikes
		animpiv	LarmL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike8
		Type	local
		At	Spikes
		animpiv	LarmR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 20
	End

	Event
		Ename	Spike9
		Type	local
		At	Spikes
		animpiv	HandL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike10
		Type	local
		At	Spikes
		animpiv	HandR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 16
	End

	Event
		Ename	Spike11
		Type	local
		At	Spikes
		animpiv	FootL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike12
		Type	local
		At	Spikes
		animpiv	FootR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 14
	End

	Event
		Ename	Spike13
		Type	local
		At	Spikes
		animpiv	ToeL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike14
		Type	local
		At	Spikes
		animpiv	ToeR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 20
	End

	Event
		Ename	Spike15
		Type	local
		At	Spikes
		animpiv	Face
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike16
		Type	local
		At	Spikes
		animpiv	Hair
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailLightGreenBitsB.part
		Part5	:QuillTrailLightBlackBitsB.part
		LifeSpan 16
	End
End

#########################################################

End