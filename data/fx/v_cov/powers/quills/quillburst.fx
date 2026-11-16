#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 87

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	HandL
		Sound ThornRumble 70 70 .8
	End
End

Condition
	On 	Time
	Time 	44

	Event
		Type	Local
		At	HandL
		Sound thornblast5 80 80 .95
	End
End

#############################################################


Condition
	On 	Time
	Time 	5

	Event
		EName	Spike
		Type	Local
		At	HandL
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	V_Quill_WepL
	End
End

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

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Spike2
		Type	Local
		At	HandR
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	V_Quill_WepR
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

#############################################################

Condition
	On 	Time
	Time 	3

	Event
		EName	Spike02b
		Type	Local
		At	waist
		Bhvr	Behaviors\PoisonSpikeScale1.bhvr
		Geom	V_Quill_Waist
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName	Spike02b
		Type	Local
		At	chest
		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	V_Quill_Chest01

	End

	Event
		EName	Spike02b
		Type	Local
		At	chest
		Bhvr	Behaviors\PoisonSpikeScaleQuick.bhvr
		Geom	V_Quill_chest02
	End
End

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		EName	Spike02b
		Type	Local
		At	UlegL
		Bhvr	Behaviors\PoisonSpikeScale1.bhvr
		Geom	V_Quill_UlegL
	End

	Event
		EName	Spike02b
		Type	Local
		At	UlegR
		Bhvr	Behaviors\PoisonSpikeScale2.bhvr
		Geom	V_Quill_UlegR
	End
End

#############################################################

Condition
	On 	Time
	Time 	3

	Event
		EName	Spike02b
		Type	Local
		At	LlegR

		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	V_Quill_LlegR

	End

	Event
		EName	Spike02b
		Type	Local
		At	LlegL

		Bhvr	Behaviors\PoisonSpikeScale.bhvr
		Geom	V_Quill_LlegL
	End


End

#############################################################

Condition
	On 	Time
	Time 	48

	Event
		Ename	Spikes
		Type	start
		At	chest
		Anim	Fx_SpineBurst
		Bhvr	Behaviors\QuilFade.bhvr
		LifeSpan	11

	End

	Event
		Ename	Spike1
		Type	local
		At	Spikes
		animpiv	Belt
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	14

	End

	Event
		Ename	Spike1
		Type	local
		At	Spikes
		animpiv	Waist
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	10

	End

	Event
		Ename	Spike2
		Type	local
		At	Spikes
		animpiv	Chest
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	20
	End

	Event
		Ename	Spike3
		Type	local
		At	Spikes
		animpiv	Neck
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	10
	End

	Event
		Ename	Spike4
		Type	local
		At	Spikes
		animpiv	Head
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	16
	End

	Event
		Ename	Spike5
		Type	local
		At	Spikes
		animpiv	UarmL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	10
	End

	Event
		Ename	Spike6
		Type	local
		At	Spikes
		animpiv	UarmR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	14
	End

	Event
		Ename	Spike7
		Type	local
		At	Spikes
		animpiv	LarmL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	10
	End

	Event
		Ename	Spike8
		Type	local
		At	Spikes
		animpiv	LarmR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	20
	End

	Event
		Ename	Spike9
		Type	local
		At	Spikes
		animpiv	HandL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	10
	End

	Event
		Ename	Spike10
		Type	local
		At	Spikes
		animpiv	HandR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	16
	End

	Event
		Ename	Spike11
		Type	local
		At	Spikes
		animpiv	FootL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	10
	End

	Event
		Ename	Spike12
		Type	local
		At	Spikes
		animpiv	FootR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	14
	End

	Event
		Ename	Spike13
		Type	local
		At	Spikes
		animpiv	ToeL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	10
	End

	Event
		Ename	Spike14
		Type	local
		At	Spikes
		animpiv	ToeR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan	20
	End

	Event
		Ename	Spike15
		Type	local
		At	Spikes
		animpiv	Face
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 10
	End

	Event
		Ename	Spike16
		Type	local
		At	Spikes
		animpiv	Hair
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 16
	End


End


########################################################
##2nds
##################################################

Condition
	On 	Time
	Time 	56.5

	Event
		Ename	Spikes2
		Type	start
		At	chest
		Anim	Fx_SpineBurst
		Bhvr	Behaviors\QuilFade.bhvr
		LifeSpan 10

	End

######################################################
##Gasses
########################################

	Event
		Ename	Spike1
		Type	local
		At	Spikes2
		animpiv	Belt
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 12
	End

	Event
		Ename	Spike1
		Type	local
		At	Spikes2
		animpiv	Waist
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike2
		Type	local
		At	Spikes2
		animpiv	Chest
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 12
	End

	Event
		Ename	Spike3
		Type	local
		At	Spikes2
		animpiv	Neck
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike4
		Type	local
		At	Spikes2
		animpiv	Head
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 20
	End

	Event
		Ename	Spike5
		Type	local
		At	Spikes2
		animpiv	UarmL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike6
		Type	local
		At	Spikes2
		animpiv	UarmR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 14
	End

	Event
		Ename	Spike7
		Type	local
		At	Spikes2
		animpiv	LarmL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike8
		Type	local
		At	Spikes2
		animpiv	LarmR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 20
	End

	Event
		Ename	Spike9
		Type	local
		At	Spikes2
		animpiv	HandL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike10
		Type	local
		At	Spikes2
		animpiv	HandR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 14
	End

	Event
		Ename	Spike11
		Type	local
		At	Spikes2
		animpiv	FootL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike12
		Type	local
		At	Spikes2
		animpiv	FootR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 20
	End

	Event
		Ename	Spike13
		Type	local
		At	Spikes2
		animpiv	ToeL
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike14
		Type	local
		At	Spikes2
		animpiv	ToeR
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 12
	End

	Event
		Ename	Spike15
		Type	local
		At	Spikes2
		animpiv	Face
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 8
	End

	Event
		Ename	Spike16
		Type	local
		At	Spikes2
		animpiv	Hair
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part4	:QuillTrailGreenBitsB.part
		Part5	:QuillTrailBlackBitsB.part
		LifeSpan 14
	End
End

#############################################################

End