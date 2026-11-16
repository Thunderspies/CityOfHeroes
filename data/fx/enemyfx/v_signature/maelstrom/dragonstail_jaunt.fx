#############################################################
## MonkeyKick.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

#Condition
#	On	Time
#	Time	3
#
#	Event
#		ENAME	Jump
#		Type	Local
#		At	FootR
#		part1	CustomizeablePowers\MartialArts\MAKickStreak02.part
#		Sound Monkey4 60 60 0.9
#		LifeSpan 20
#	End
#End
#
#Condition
#	On	Time
#	Time	10
#
#	Event
#		ENAME	KickStreak2
#		Type	Local
#		At	FootR
#		part1	CustomizeablePowers\MartialArts\MAKickStreak01.part
#		part2	CustomizeablePowers\MartialArts\MAMonkeyKickStreak01.part
#		part3	CustomizeablePowers\MartialArts\MAKickStar01.part
#		LifeSpan 12
#	End
#End
#
#Condition
#	On	Time
#	Time	10
#
#	Event
#		ENAME	KickStreakLrg
#		Type	Local
#		At	FootR
#		part1	CustomizeablePowers\MartialArts\MonkeyKickShard.part
#		part2	CustomizeablePowers\MartialArts\MonkeyKickSparkles.part
#		LifeSpan 12
#	End
#End
#
##############################################################
#
#Condition
#	On	Time
#	Time	17
#
#	Event
#		Type	Local
#		At	Target
#		Sound Hit6 100.0 100.0 0.9
#		LifeSpan 20
#	End
#End

#############################################################

Condition
	On	Time
	Time	11

	Event
		EName	TargetRootOffset
		Type	Local
		At	T_Root
		BhvrOverride
			PositionOffset	0 0 6
		End
	End

	Event
		EName	TargetLookAt
		Type	StartPositOnly
		BhvrOverride
		#	PYRRotate	0 180 0
			PYRRotateJitter	0 45 0
		End
		At	TargetRootOffset
		LookAt	T_Root
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	Start
		At	TargetLookAt
		Bhvr	ENEMYFX\V_Signature\Maelstrom\FX_JauntFade.bhvr
		Anim	FX_MaelstromJaunt_DragonsTail
		Lifespan 20
	End
End

#############################################################

End