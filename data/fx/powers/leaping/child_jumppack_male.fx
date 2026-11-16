#############################################################
## JumpPack.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	chest
		Bhvr	POWERS\Leaping\JumpPack_Male.bhvr
		Geom	GEO_Back_Jump_Pack
	End

	Event
		EName 	JetHookup
		Type	local
		At	chest
		Bhvr	POWERS\Leaping\JumpPack_Male.bhvr
		Geom	JumpPack_Hookup
	End
End

#############################################################

Condition
	On	TriggerBits
	TriggerBits	JUMP
	DoMany		1

	Event
		Ename	Jet1
		Type	local
		At	JetHookup
		While	JUMP
		Flags	OneAtATime
		Altpiv	1
		Bhvr	Behaviors\fadeoutShort.bhvr
		BhvrOverride
			PositionOffset  0 -0.08 0
			PYRrotate	-30 0 0
		End
		Part	:JumpPackThrusterCore.part
		Part	:JumpPackFlameLicks.part
		Part	:JumpPackThrusterGases.part
		Part	:JumpPackThrusterCoreBurst.part
		SoundNoRepeat 2
		Sound JumpPack_Burst_01 50 50 .9
		Sound JumpPack_Burst_02 50 50 .9
		Sound JumpPack_Burst_03 50 50 .9
		Sound JumpPack_Burst_04 50 50 .9
	End

	Event
		Ename	Jet2
		Type	local
		At	JetHookup
		While	jump
		Flags	OneAtATime
		Altpiv	2
		Bhvr	Behaviors\fadeoutShort.bhvr
		BhvrOverride
			PositionOffset  0 -0.08 0
			PYRrotate	-30 0 10
		End
		Part	:JumpPackThrusterCoreSmall.part
		Part	:JumpPackFlameLicksSmall.part
		Part	:JumpPackThrusterGases.part
		Part	:JumpPackThrusterCoreSmallBurst.part
		Sound JumpPack_Loop 50 50 .5
		bhvr	behaviors/sound/SoundOut1.bhvr
	End

	Event
		Ename	Jet3
		Type	local
		At	JetHookup
		While	jump
		Flags	OneAtATime
		Altpiv	3
		Bhvr	Behaviors\fadeoutShort.bhvr
		BhvrOverride
			PositionOffset  0 -0.06 0
			PYRrotate	-30 0 -10
		End
		Part	:JumpPackThrusterCoreSmall.part
		Part	:JumpPackFlameLicksSmall.part
		Part	:JumpPackThrusterGases.part
		Part	:JumpPackThrusterCoreSmallBurst.part
	End

	Event
		Ename	Jet4
		Type	local
		At	JetHookup
		While	jump
		Flags	OneAtATime
		Altpiv	4
		Bhvr	Behaviors\fadeoutShort.bhvr
		BhvrOverride
			PositionOffset  0 -.06 0
			PYRrotate	-30 0 20
		End
		Part	:JumpPackThrusterCoreSmallest.part
		Part	:JumpPackFlameLicksSmallest.part
		Part	:JumpPackThrusterGases.part
		Part	:JumpPackThrusterCoreSmallestBurst.part
	End

	Event
		Ename	Jet5
		Type	local
		At	JetHookup
		While	jump
		Flags	OneAtATime
		Altpiv	5
		Bhvr	Behaviors\fadeoutShort.bhvr
		BhvrOverride
			PositionOffset  0 -.05 0
			PYRrotate	-30 0 -20
		End
		Part	:JumpPackThrusterCoreSmallest.part
		Part	:JumpPackFlameLicksSmallest.part
		Part	:JumpPackThrusterGases.part
		Part	:JumpPackThrusterCoreSmallestBurst.part
	End
End

#############################################################

End