#########################################################
## Spines - Quills
#########################################################

FxInfo

Flags InheritAlpha

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	HandL
		Sound Quills1 70 70 0.7
	End
End

Condition
	On 	cycle
	Time 	45

	Event
		Type	start
		At	chest
		Sound QuillExplo2 90 90 0.25
		Lifespan 30
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

## REPEATING SPINE BURST #######################################################

Condition
	On 	cycle
	Time 	45

	Event
		Ename	Spikes
		Type	Create
		Inherit	Position
		At	Chest
		Bhvr	Behaviors\QuilFade2.bhvr
		Anim	Fx_QuillBurst_Large
		LifeSpan 10
	End

	Event
		Ename	Spike1
		Type	local
		At	Spikes
		animpiv	Belt
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 9
	End

	Event
		Ename	Spike1
		Type	local
		At	Spikes
		animpiv	Waist
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 12
	End

	Event
		Ename	Spike2
		Type	local
		At	Spikes
		animpiv	Chest
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 16
	End

	Event
		Ename	Spike3
		Type	local
		At	Spikes
		animpiv	Neck
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 9
	End

	Event
		Ename	Spike4
		Type	local
		At	Spikes
		animpiv	Head
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 12
	End

	Event
		Ename	Spike5
		Type	local
		At	Spikes
		animpiv	UarmL
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 9
	End

	Event
		Ename	Spike6
		Type	local
		At	Spikes
		animpiv	UarmR
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 16
	End

	Event
		Ename	Spike7
		Type	local
		At	Spikes
		animpiv	LarmL
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 12
	End

	Event
		Ename	Spike8
		Type	local
		At	Spikes
		animpiv	LarmR
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 6
	End

	Event
		Ename	Spike9
		Type	local
		At	Spikes
		animpiv	HandL
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 9
	End

	Event
		Ename	Spike10
		Type	local
		At	Spikes
		animpiv	HandR
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 22
	End

	Event
		Ename	Spike11
		Type	local
		At	Spikes
		animpiv	FootL
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 9
	End

	Event
		Ename	Spike12
		Type	local
		At	Spikes
		animpiv	FootR
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 12
	End

	Event
		Ename	Spike13
		Type	local
		At	Spikes
		animpiv	ToeL
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 22
	End

	Event
		Ename	Spike14
		Type	local
		At	Spikes
		animpiv	ToeR
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 9
	End

	Event
		Ename	Spike15
		Type	local
		At	Spikes
		animpiv	Face
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 6
	End

	Event
		Ename	Spike16
		Type	local
		At	Spikes
		animpiv	Hair
		Part2	:QuillTrailBlackBitsB.part
		Part2	:QuillTrailGreenBitsB.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
		LifeSpan 12
	End
End

#Condition
#	On 	cycle
#	Time 	45
#
#	Event
#		Ename	Spikesd
#		Type	start
#		At	chest
#		Bhvr	Behaviors\QuilFade3.bhvr
#		Anim	Fx_quillburst_Large2
#		LifeSpan 13
#	End
#End

#########################################################

End