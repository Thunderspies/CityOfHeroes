#########################################################
## Umbral Blast - Animate Foe
#########################################################

FxInfo

Lifespan 330


## GROUND FX ######################################################

Condition
	On	Time
	Time	45

	Event
		Type	start
		At	Mystic
		ChildFX	Scripted\RomulusNictus\Child_NictusEnergy_Ground.fx
		Lifespan 135
	End
End

## HIT 1 ###########################################################

Condition
	On	Time
	Time	20

	Event
		Type	start
		At	Root
		ChildFX	Scripted\RomulusNictus\Child_RomulusNictus_Impact.fx
	End
End

### HIT 2 ###########################################################

Condition
	On	Time
	Time	42

	Event
		Type	start
		At	Root
		ChildFX	Scripted\RomulusNictus\Child_RomulusNictus_Impact.fx
	End
End

### HIT 3 ###########################################################

Condition
	On	Time
	Time	80

	Event
		Type	start
		At	Root
		ChildFX	Scripted\RomulusNictus\Child_RomulusNictus_Impact.fx
	End
End

#### HIT 4 ###########################################################
#
#Condition
#	On	Time
#	Time	105
#
#	Event
#		Type	start
#		At	Root
#		ChildFX	Scripted\RomulusNictus\Child_RomulusNictus_Impact.fx
#	End
#End

## NICTUS BODY ARUA ###########################################################

Condition
	On	Time
	Time	50

	Event
		Type	start
		At	Root
		ChildFX	Scripted\RomulusNictus\Child_RomulusNictus_BodyAura.fx
	End
End

## NICTUS EXPLOSION ###########################################################

Condition
	On	Time
	Time	137

	Event
		Type	start
		At	Root
		ChildFX	Scripted\RomulusNictus\Child_NictusEnergy_Explosion.fx
	End
End

## ROMULUS NICTUS AURA ######################################################

Condition
	On	Time
	Time	140

	Event
		EName 	Eyes
		Type	Local
		At	Root
		ChildFX	Scripted\RomulusNictus\Child_RomulusNictus_Eyes.fx
	End
End

## NICTUS STOMP ###########################################################

Condition
	On	Time
	Time	290

	Event
		Type	start
		At	Mystic
		ChildFX	Scripted\RomulusNictus\Child_NictusEnergy_Stomp.fx
	End
End

#############################################################

End