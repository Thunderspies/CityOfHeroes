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
		At	Head
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

##################################

End
