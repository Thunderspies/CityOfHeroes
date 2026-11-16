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
		Ename	Shadow
		Type	Local
		At	BigMeteor
		BhvrOverride
			PositionOffset	0 0.5 0
			PyrRotateJitter	0 180 0
		End
		Part	:Meteor_Attack_Shadow.part
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
	Time	70
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
	Time	75
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
	Time	80
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
	Time	85
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

	Event
		Ename	Glow
		Type	Local
		At	BigMeteor
		BhvrOverride
			PositionOffset	0 0.5 0
			PyrRotateJitter	0 180 0
		End
		Part	:Meteor_Attack_Glow.part
		Lifespan 25
	End
End

Condition
	On	Time
	Time	90

	Event
		Ename	Glow
		Type	Local
		At	BigMeteor
		BhvrOverride
			PositionOffset	0 0.5 0
			PyrRotateJitter	0 180 0
		End
		Part	:Meteor_Attack_Glow.part
		Lifespan 15
	End
End

#############################################################

End