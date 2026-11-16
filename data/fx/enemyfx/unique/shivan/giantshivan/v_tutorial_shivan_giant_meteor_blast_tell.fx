#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 240

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	BigMeteor
		Type	Start
		At	T_Root
	End


	Event
		Ename	Pebbles
		Type	Start
		At	T_Root
	End

End


Condition
	On	Time
	Time	0

	Event
		Ename	Glow
		Type	Local
		At	BigMeteor
		BhvrOverride
			PositionOffset	0 0.5 0
			PyrRotateJitter	0 180 0
		End
		Part	:Meteor_Attack_Glow.part
		Part	:Meteor_Attack_Glow.part
		Lifespan 220
	End
End

Condition
	On	Time
	Time	20
	Chance	0.8

	Event
		Ename	pebble1
		Type	Local
		At	BigMeteor
		ChildFX	:Meteor_Attack_MiniCHILD.fx
	End

End


Condition
	On	Time
	Time	30
	Chance	0.8

	Event
		Ename	pebble1
		Type	Local
		At	BigMeteor
		ChildFX	:Meteor_Attack_MiniCHILD.fx
	End

	Event
		Type	Local
		At	Root
		Sound 	ShivanGiant_MeteorFall_01 200 150 .6
	End
End


Condition
	On	Time
	Time	50
	Chance	0.8

	Event
		Ename	pebble1
		Type	Local
		At	BigMeteor
		ChildFX	:Meteor_Attack_MiniCHILD.fx
	End
End


Condition
	On	Time
	Time	60
	Chance	0.8

	Event
		Ename	pebble1
		Type	Local
		At	BigMeteor
		ChildFX	:Meteor_Attack_MiniCHILD.fx
	End
End

Condition
	On	Time
	Time	90
	Chance	0.8

	Event
		Ename	pebble1
		Type	Local
		At	BigMeteor
		ChildFX	:Meteor_Attack_MiniCHILD.fx
	End
End

Condition
	On	Time
	Time	100
	Chance	0.8

	Event
		Ename	pebble1
		Type	Local
		At	BigMeteor
		ChildFX	:Meteor_Attack_MiniCHILD.fx
	End
End

#Condition
#	On	Time
#	Time	60
#
#	Event
#		Ename	Glow
#		Type	Local
#		At	BigMeteor
#		BhvrOverride
#			PositionOffset	0 0.5 0
#			PyrRotateJitter	0 180 0
#		End
#		Part	:Meteor_Attack_Glow.part
#		Lifespan 25
#	End
#End
#
#Condition
#	On	Time
#	Time	140
#
#	Event
#		Ename	Glow
#		Type	Local
#		At	BigMeteor
#		BhvrOverride
#			PositionOffset	0 0.5 0
#			PyrRotateJitter	0 180 0
#		End
#		Part	:Meteor_Attack_Glow.part
#		Lifespan 30
#	End
#End
##
#Condition
#	On	Time
#	Time	170
#
#	Event
#		Ename	Glow
#		Type	Local
#		At	BigMeteor
#		BhvrOverride
#			PositionOffset	0 0.5 0
#			PyrRotateJitter	0 180 0
#		End
#		Part	:Meteor_Attack_Glow.part
#		Lifespan 30
#	End
#End

#############################################################

End