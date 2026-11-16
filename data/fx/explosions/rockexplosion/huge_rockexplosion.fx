#########################################################
##	onfire
##
FxInfo  

LifeSpan	100

######  offset markers  ##############################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	FootL
		Bhvr	Behaviors\stone_offset_1.bhvr
	End

End


######  expolsions  ##############################

Condition
	On 	Time
	Time 	1

	Event
		Type	Start
		At	Prime
		ChildFx	Explosions\RockExplosion\RockExplosion.Fx
		ChildFx	V_COV\World\Vault\VaultExplosion.fx
	End

End

End			