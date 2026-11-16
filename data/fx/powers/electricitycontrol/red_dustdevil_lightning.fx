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

	End

#	Event
#		Type	local
#		At	Hookup
#		Part	:LightningStrike2.part	
#		Pmagnet	 root
#
#	End	

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
			Startjitter	2 0 2
			PositionOffset	0 50 0
		End

	End
	
	Event

		Ename	ChargedBolt1
		Type	start
		At	node01
		altpiv	1
		
		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 40 0
		End

	End

	Event

		Ename	ChargedBolt2
		Type	start
		At	node02
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 30 0
		End

	End

	Event

		Ename	ChargedBolt3
		Type	start
		At	node03
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 20 0
		End

	End

	Event

		Ename	ChargedBolt4
		Type	start
		At	node04
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 10 0
		End

	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
		POther	 root

	End

End


#######################################################################################################


Condition
	On 	Time
	Time 	0

	Event

		Ename	node01
		Type	start
		At	Root
		Geom	centerdummy

		BhvrOverride
			Startjitter	2 0 2
			PositionOffset	0 50 0
		End

	End
	
	Event

		Ename	ChargedBolt1
		Type	start
		At	node01
		altpiv	1
		
		Part	:Red_ChargedBolt01b.Part	
		#Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 40 0
		End

	End

	Event

		Ename	ChargedBolt2
		Type	start
		At	node02
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		#Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 30 0
		End

	End

	Event

		Ename	ChargedBolt3
		Type	start
		At	node03
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		#Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 20 0
		End

	End

	Event

		Ename	ChargedBolt4
		Type	start
		At	node04
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		#Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
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
			Startjitter	2 0 2
			PositionOffset	0 10 0
		End

	End

	Event

		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1

		Part	:Red_ChargedBolt01b.Part	
		#Part	:Red_ChargedBolt03.Part
		Part	:Red_ChargedBolt01b.Part	
		Part	:Red_ChargedBolt02b.Part
		Part	:Red_ChargedBolt05b.Part	
		Part	:Red_ChargedBolt06b.Part
		POther	 root

	End

End

######################################################################################


End