#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	65    

########################################################


######################################################################################



Condition
	On 	Time
	Time 	0
	
	Event
		Ename	Hookup
		Type	local
		At	Root
		BHVR	Powers/ItemsAndTempPowers/OverheadOffset.bhvr
		Sound SSthunder2 180 180 .9

	End

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike2.part	
		Pmagnet	 root

	End	

######################################################################################

	Event
		Ename	Hookup2
		Type	local
		At	Root
		BHVR	Powers/ItemsAndTempPowers/OverheadOffset2.bhvr

	End

	Event

		Ename	node01
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 50 0
		End

	End
	
	Event

		Ename	ChargedBolt1
		Type	start
		At	node01
		altpiv	1
		
		Part	:Red_ChargedBolt01a.Part	
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part	
		Part	:Red_ChargedBolt06a.Part
		POther	node02

	End	

End

Condition
#	On	Time
#	Time	1

	Event

		Ename	node02
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 40 0
		End

	End

	Event

		Ename	ChargedBolt2
		Type	start
		At	node02
		altpiv	1

		Part	:Red_ChargedBolt01a.Part	
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part	
		Part	:Red_ChargedBolt06a.Part
		POther	 node03

	End

End

Condition
#	On	Time
#	Time	1

	Event

		Ename	node03
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 30 0
		End

	End

	Event

		Ename	ChargedBolt3
		Type	start
		At	node03
		altpiv	1

		Part	:Red_ChargedBolt01a.Part	
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part	
		Part	:Red_ChargedBolt06a.Part
		POther	 node04

	End

End

Condition
#	On	Time
#	Time	1

	Event

		Ename	node04
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 20 0
		End

	End

	Event

		Ename	ChargedBolt4
		Type	start
		At	node04
		altpiv	1

		Part	:Red_ChargedBolt01a.Part	
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part	
		Part	:Red_ChargedBolt06a.Part
		POther	 node05

	End

End


Condition
#	On	Time
#	Time	1

	Event

		Ename	node05
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 10 0
		End

	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1

		Part	:Red_ChargedBolt01a.Part	
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part	
		Part	:Red_ChargedBolt06a.Part
		POther	 root

	End

End

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
	
#	Event
#		Type	local
#		At	Hookup
#		Part	:LightningStrikeInvert.part
#		Pmagnet	 root
#
#	End	

#######################################################################################

#
#	Event
#
#		Ename	node01a
#		Type	start
#		At	Root
#		Geom	centerdummy
#
#		BhvrOverride
#			Startjitter	3 0 3
#			PositionOffset	0 50 0
#		End
#
#	End
#	
#	Event
#
#		Ename	ChargedBolt1
#		Type	start
#		At	node01a
#		altpiv	1
#		
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt03.Part
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt02a.Part
#		Part	:Red_ChargedBolt05a.Part	
#		Part	:Red_ChargedBolt06a.Part
#		POther	node02a
#
#	End	
#
#End
#
#Condition
##	On	Time
##	Time	1
#
#	Event
#
#		Ename	node02a
#		Type	start
#		At	Root
#		Geom	centerdummy
#
#		BhvrOverride
#			Startjitter	3 0 3
#			PositionOffset	0 40 0
#		End
#
#	End
#
#	Event
#
#		Ename	ChargedBolt2
#		Type	start
#		At	node02a
#		altpiv	1
#
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt03.Part
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt02a.Part
#		Part	:Red_ChargedBolt05a.Part	
#		Part	:Red_ChargedBolt06a.Part
#		POther	 node03a
#
#	End
#
#End
#
#Condition
##	On	Time
##	Time	1
#
#	Event
#
#		Ename	node03a
#		Type	start
#		At	Root
#		Geom	centerdummy
#
#		BhvrOverride
#			Startjitter	3 0 3
#			PositionOffset	0 30 0
#		End
#
#	End
#
#	Event
#
#		Ename	ChargedBolt3
#		Type	start
#		At	node03a
#		altpiv	1
#
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt03.Part
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt02a.Part
#		Part	:Red_ChargedBolt05a.Part	
#		Part	:Red_ChargedBolt06a.Part
#		POther	 node04a
#
#	End
#
#End
#
#Condition
##	On	Time
##	Time	1
#
#	Event
#
#		Ename	node04a
#		Type	start
#		At	Root
#		Geom	centerdummy
#
#		BhvrOverride
#			Startjitter	3 0 3
#			PositionOffset	0 20 0
#		End
#
#	End
#
#	Event
#
#		Ename	ChargedBolt4
#		Type	start
#		At	node04a
#		altpiv	1
#
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt03.Part
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt02a.Part
#		Part	:Red_ChargedBolt05a.Part	
#		Part	:Red_ChargedBolt06a.Part
#		POther	 node05a
#
#	End
#
#End
#
#
#Condition
##	On	Time
##	Time	1
#
#	Event
#
#		Ename	node05a
#		Type	start
#		At	Root
#		Geom	centerdummy
#
#		BhvrOverride
#			Startjitter	3 0 3
#			PositionOffset	0 10 0
#		End
#
#	End
#
#	Event
#
#		Ename	ChargedBolt6
#		Type	start
#		At	node05a
#		altpiv	1
#
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt03.Part
#		Part	:Red_ChargedBolt01a.Part	
#		Part	:Red_ChargedBolt02a.Part
#		Part	:Red_ChargedBolt05a.Part	
#		Part	:Red_ChargedBolt06a.Part
#		POther	 root
#
#	End
#
#######################################################################################################

	Event

		Ename	node01
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 50 0
		End

	End
	
	Event

		Ename	ChargedBolt1
		Type	start
		At	node01
		altpiv	1
		
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
#	On	Time
#	Time	1

	Event

		Ename	node02
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 40 0
		End

	End

	Event

		Ename	ChargedBolt2
		Type	start
		At	node02
		altpiv	1

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
#	On	Time
#	Time	1

	Event

		Ename	node03
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 30 0
		End

	End

	Event

		Ename	ChargedBolt3
		Type	start
		At	node03
		altpiv	1

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
#	On	Time
#	Time	1

	Event

		Ename	node04
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 20 0
		End

	End

	Event

		Ename	ChargedBolt4
		Type	start
		At	node04
		altpiv	1

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
#	On	Time
#	Time	1

	Event

		Ename	node05
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	3 0 3
			PositionOffset	0 10 0
		End

	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1

		Part	:Red_ChargedBolt01a.Part	
		Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01a.Part	
		Part	:Red_ChargedBolt02a.Part
		Part	:Red_ChargedBolt05a.Part	
		Part	:Red_ChargedBolt06a.Part
		POther	 root

	End

End

######################################################################################

Condition
	On 	Time
	Time 	2
	Event
		Type	local
		At	Chest
		Part	:LightningStrikeGlow.part
		Part	:LightningStrikeHaze.part
		Part	:LightningStrikeHazeWhite.part
		Lifespan		10
	End	

End


Condition
	On 	Time
	Time 	3

	Event
		Type	local
		At	Chest	
		Part	:LightningStreeks.part
		Part	:LightningStreeksDark.part

		Lifespan		12
	End

End



End