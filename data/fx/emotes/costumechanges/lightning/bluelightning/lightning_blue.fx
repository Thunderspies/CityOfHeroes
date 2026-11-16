#############################################################
## Lightning_Red.fx
#############################################################

FxInfo

LifeSpan 165

#############################################################

#Condition
#	On 	Time
#	Time 	14
#
#	Event
#		Ename	CameraShake
#		Type	local
#		At	Root
#		Bhvr	Behaviors\SSLSShake.bhvr
#	#	Sound	lightningbolt4 120 30 .9
#	End
#End

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
	Time 	0

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
		Part	:Red_ChargedBolt01a.Part
		POther	node02
		Lifespan 7
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
		Part	:Red_ChargedBolt01a.Part
		POther	 node03
		Lifespan 7
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
		Part	:Red_ChargedBolt01a.Part
		POther	 node04
		Lifespan 7
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
		Part	:Red_ChargedBolt01a.Part

		POther	 node05
		Lifespan 7
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
		Part	:Red_ChargedBolt01a.Part
		POther	 WepR
		Lifespan 7

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
		Part	:Red_ChargedBolt01a.Part
		POther	node02
		Lifespan 7
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
		Part	:Red_ChargedBolt01a.Part
		POther	 node03
		Lifespan 7
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
		Part	:Red_ChargedBolt01a.Part
		POther	 node04
		Lifespan 7
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
		Part	:Red_ChargedBolt01a.Part
		POther	 node05
		Lifespan 7

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
		Lifespan 7
	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1
		Part	:Red_LightningRod_ChargedBoltCore.Part
		Part	:Red_LightningRod_ChargedBoltCore2.Part
		Part	:Red_ChargedBolt01a.Part
		POther	 WepR
		Lifespan 7
	End
End


#############################################################

Condition
	On 	Time
	Time 	4
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
	Time 	5

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