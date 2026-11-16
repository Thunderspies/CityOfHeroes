
#########################################################################################
########	Notes: Recluse Ranged cone energy attack consists if a few child fx.
########               Buildup - building energy in character
########               ProjectileCone - exploding energy mass
########               
########               
########               
#########################################################################################

FxInfo
	
LifeSpan	90

##################################################################################
####################  P R I M E   decoy for early splat ################  01  ####
##################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	Buildup
		Type	Local
		At	Chest
		
		ChildFX V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_Glow_BuildupRed.fx
	End

	Event
		EName	EmitterPos
		Type	Local
		At	Belt
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltEmitter.bhvr
##		Geom	Testing_Target
	End
End


Condition
	On	Time
	Time	31

	Event
		EName	BurstLegs
		Type	Local
		At	Root

		ChildFX V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_Glow_BackpackFinalBurst.fx
	End
End

Condition
	On	Time
	Time	35
	
	Event
		EName	ProjectileCone
		Type	Local
		At	Root

		ChildFX V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_Energy_ConeCloud.fx
	End		
	
	Event
		EName	Prime
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\B_Projectile_ConeTravel.bhvr
#		Geom	Testing_Target
		Magnet	T_Chest
		LookAt	T_Chest
	End
End


End