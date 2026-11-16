#########################################################
## Blue Mitochondria - Mez attack
########################################################

FxInfo

Lifespan 450

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	Local
		At	Hips
		Bhvr	ENEMYFX\Romans\Nictus\Nictus.bhvr
	End
End

## BEAM ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Beam
		Type	Local
		At	Offset
		ChildFX	Scripted\RomulusNictus\Child_Nictus_Beam.fx
	End
End

## PULSE 1 ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Projectile
		Type	Local
		At	Offset
		ChildFX	Scripted\RomulusNictus\Child_Nictus_Projectile.fx
		Lifespan 90
	End
End

## PULSE 2 ###########################################################

Condition
	On	Time
	Time	22

	Event
		EName	Projectile
		Type	Local
		At	Offset
		ChildFX	Scripted\RomulusNictus\Child_Nictus_Projectile.fx
		Lifespan 90
	End
End

### PULSE 3 ###########################################################

#Condition
#	On	Time
#	Time	60
#
#	Event
#		EName	Projectile
#		Type	Posit
#		At	Root
#		ChildFX	Scripted\RomulusNictus\Child_Nictus_Projectile.fx
#		Lifespan 90
#	End
#End


### BIG PULSE ###########################################################

Condition
	On	Time
	Time	85

	Event
		EName	Projectile
		Type	Local
		At	Offset
		ChildFX	Scripted\RomulusNictus\Child_Nictus_BigProjectile.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors\invisible.bhvr
		Flags	AdoptParentEntity
	End
End

Condition
	On	Time
	Time	100

	Event
		Type	Destroy
		EName	Beam
	End
End

#############################################################

End