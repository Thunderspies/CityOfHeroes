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
	Time 	30


	Event
		EName 	Prime
		Type	start
		At	T_Chest
		Bhvr	CustomizeablePowers\DarkMiasma\SoulTransfer1.bhvr
		BhvrOverride
			TrackRate	1.5
		End
		Part	:Projectile_Bright_Head.part
		Part	:Projectile_Body.part
		Magnet	Chest
		Lookat	Chest

	End

	Event
		EName 	Prime2
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0.0 0.0 2
		End
		Part	:Projectile_Body_Fill.part
	End
End

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

