#############################################################
## Disintegration_Attack.fx
#############################################################

FxInfo
Lifespan 300

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	TentacleAnchor
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 1.0 0.0 0.0
		End
	End

	Event
		EName 	TentacleAnchor2
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset -0.7 -1.0 2.0
		End
	End

	Event
		EName 	TentacleAnchor3
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset -2.3 0.7 0.5
		End
	End
End
Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		Sound 	FS_PsionicBlast_Attack_01 100 100 .8
	End
End

#########################################################

Condition
	On 	Time
	Time 	53

	Event
		EName 	Prime
		Type	start
		At	TentacleAnchor2
		Bhvr	CustomizeablePowers\DarkMiasma\SoulTransfer1.bhvr
		BhvrOverride
			TrackRate	1.5
		End
		Part	:PsionicBlast_Lightning.part
		Part	:PsionicBlast_Streamers.part
		Part	:PsionicBlast_Streamers2.part

		Magnet	T_Chest
		Lookat	T_Chest

	End

	Event
		EName	Prime2
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0.0 0.0 2
		End
		Part	:Projectile_Bright_Head.part
		Part	:Projectile_Body_Core.part
		Part	:Projectile_Body_Fill.part
		Part	:Projectile_Body_Fill_ALpha.part

	End
End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime3
		Type	Start
		At	TentacleAnchor
		Bhvr	CustomizeablePowers\DarkMiasma\SoulTransfer1.bhvr
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
			TrackRate	1.5
		End
		Part	:PsionicBlast_Lightning.part
		Part	:PsionicBlast_Streamers.part
		Part	:PsionicBlast_Streamers2.part

		Magnet	T_Chest
		Lookat	T_Chest

	End

	Event
		EName 	Prime4
		Type	Local
		At	Prime3
		BhvrOverride
			PositionOffset	0.0 0.0 2
		End
		Part	:Projectile_Bright_Head.part
		Part	:Projectile_Body_Core.part
		Part	:Projectile_Body_Fill.part
		Part	:Projectile_Body_Fill_ALpha.part

	End
End

Condition
	On 	Time
	Time 	57

	Event
		EName 	Prime5
		Type	Start
		At	TentacleAnchor3
		Bhvr	CustomizeablePowers\DarkMiasma\SoulTransfer1.bhvr
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
			TrackRate	1.5
		End
		Part	:PsionicBlast_Lightning.part
		Part	:PsionicBlast_Streamers.part
		Part	:PsionicBlast_Streamers2.part

		Magnet	T_Chest
		Lookat	T_Chest

	End

	Event
		EName 	Prime6
		Type	Local
		At	Prime5
		BhvrOverride
			PositionOffset	0.0 0.0 2
		End
		Part	:Projectile_Bright_Head.part
		Part	:Projectile_Body_Core.part
		Part	:Projectile_Body_Fill.part
		Part	:Projectile_Body_Fill_ALpha.part

	End
End

#########################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy

	End

	Event
		EName 	Prime4
		Type	Destroy
	End
End

Condition
	On 	Prime5Hit

	Event
		EName 	Prime5
		Type	Destroy

	End

	Event
		EName 	Prime6
		Type	Destroy
	End
End

##################################

End
