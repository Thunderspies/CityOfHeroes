#############################################################
## Red_LightningRodChild.fx
#############################################################

FxInfo

Lifespan 165

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local
		At	Root
		BHVR	Powers\ItemsAndTempPowers\OverheadOffset.bhvr
	End

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike2.part
		Pmagnet	 root
	End

#############################################################

	Event
		Ename	Hookup2
		Type	local
		At	Root
		BHVR	Powers\ItemsAndTempPowers\OverheadOffset2.bhvr
	End

	Event
		Ename	node01
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 300 0
		End
	End

	Event
		Ename	ChargedBolt1
		Type	start
		At	node01
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	node02
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	node02
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 200 0
		End
	End

	Event
		Ename	ChargedBolt2
		Type	start
		At	node02
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 node03
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	node03
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 120 0
		End
	End

	Event
		Ename	ChargedBolt3
		Type	start
		At	node03
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 node04
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	node04
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 60 0
		End
	End

	Event
		Ename	ChargedBolt4
		Type	start
		At	node04
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 node05
	End
End


Condition
	On	Time
	Time	0

	Event
		Ename	node05
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 30 0
		End
	End

	Event
		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 WepR
	End
End

#############################################################

Condition
	On 	Time
	Time 	3

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike3.part
		Pmagnet	 root
	End
End


Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	Hookup2
		Part	:LightningStrike2.part
		Pmagnet	 root
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike.part
		Pmagnet	 root
	End

#############################################################

	Event
		Ename	node01
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 300 0
		End
	End

	Event
		Ename	ChargedBolt1
		Type	start
		At	node01
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	node02
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	node02
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 200 0
		End
	End

	Event
		Ename	ChargedBolt2
		Type	start
		At	node02
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 node03
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	node03
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 120 0
		End
	End

	Event
		Ename	ChargedBolt3
		Type	start
		At	node03
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 node04
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	node04
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 60 0
		End
	End

	Event
		Ename	ChargedBolt4
		Type	start
		At	node04
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 node05
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	node05
		Type	start
		At	Root
		Geom	centerdummy
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 30 0
		End
	End

	Event
		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_LinghtningRodSpark.part
		Part	:Red_LinghtningRodSpark_White.part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01a.Part
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part
		Part	:Red_ChargedBolt06a.Part
		POther	 wepR
	End
End

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	local
		At	Chest
		Part	:LightningStrikeHazeSubtle.part
		Part	:LightningStrikeHazeWhiteSubtle.part
		Lifespan  10
	End
End

#############################################################

End