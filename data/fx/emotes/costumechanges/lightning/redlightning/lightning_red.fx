#############################################################
## Lightning_Red.fx
#############################################################

FxInfo

LifeSpan 165

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		Ename	CameraShake
		Type	local
		At	Root
		Bhvr	Behaviors\SSLSShake.bhvr
		Sound lightningbolt4 120 120 .9
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		Ename	Hookup
		Type	local
		At	Root
		BHVR	Powers\ItemsAndTempPowers\OverheadOffset.bhvr
	End

	Event
		Type	local
		At	Hookup
		#Part	:LightningStrike2.part
		Pmagnet	 root
	End

	Event
		Ename	Hookup2
		Type	local
		At	Root
		BHVR	Powers\ItemsAndTempPowers\OverheadOffset2.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event

		Ename	node01
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 300 0
		End
		Geom	centerdummy
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

	Event

		Ename	node02
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 200 0
		End
		Geom	centerdummy
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

	Event

		Ename	node03
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 120 0
		End
		Geom	centerdummy
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

	Event

		Ename	node04
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 60 0
		End
		Geom	centerdummy
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

	Event
		Ename	node01
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 300 0
		End
		Geom	centerdummy
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

	Event

		Ename	node02
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 200 0
		End
		Geom	centerdummy
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

	Event
		Ename	node03
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 120 0
		End
		Geom	centerdummy
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

	Event
		Ename	node04
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 60 0
		End
		Geom	centerdummy
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

	Event
		Ename	node05
		Type	start
		At	Root
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 30 0
		End
		Geom	centerdummy
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
		POther	 WepR
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike.part
		Pmagnet	 root
	End
End

Condition
	On 	Time
	Time 	31

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike3.part
		Pmagnet	 root

	End
End

Condition
	On 	Time
	Time 	33

	Event
		Type	local
		At	Hookup2
		Part	:LightningStrike2.part
		Pmagnet	 root

	End
End

#############################################################

Condition
	On 	Time
	Time 	30
	Event
		Type	local
		At	Chest
		Part	:LightningStrikeGlow.part
		Part	:LightningStrikeHaze.part
		Part	:LightningStrikeHazeWhite.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	31

	Event
		Type	local
		At	Chest
		Part	:LightningStreeks.part
		Part	:LightningStreeksDark.part
		Lifespan 12
	End
End

#############################################################

End