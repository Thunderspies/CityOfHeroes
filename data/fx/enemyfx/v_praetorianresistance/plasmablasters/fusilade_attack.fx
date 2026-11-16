#############################################################
## RobotLazerBursts.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On Time
	Time 5
	
	Event
		Type	Local
		At	WepR
		sound	RES_Pistols_FireFusillade_01 100.0 80.0 .9
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName	MuzzleFlash_Right
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.75 0 0.375
		End
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	34

	Event
		Type	Local
		At	MuzzleFlash_Right
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	Local
		At	MuzzleFlash_Right
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	MuzzleFlash_Left
		Type	Local
		At	WepL
		BhvrOverride
			PYRRotate		0 180 0
			PositionOffset		-0.75 0 0.375
		End
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	36

	Event
		Type	Local
		At	MuzzleFlash_Left
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	42

	Event
		Type	Local
		At	MuzzleFlash_Left
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Start
		At	MuzzleFlash_Right
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Projectile_Cone_01.fx
	End
End

Condition
	On 	Time
	Time 	36

	Event
		Type	Start
		At	MuzzleFlash_Right
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Projectile_Cone_01.fx
	End
End

Condition
	On 	Time
	Time 	42

	Event
		Type	Start
		At	MuzzleFlash_Right
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Projectile_Cone_01.fx
	End
End


#############################################################

Condition
	On 	Time
	Time 	33

	Event
		Type	Start
		At	MuzzleFlash_Left
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Projectile_Cone_01.fx
	End
End

Condition
	On 	Time
	Time 	39

	Event
		Type	Start
		At	MuzzleFlash_Left
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Projectile_Cone_01.fx
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	Start
		At	MuzzleFlash_Left
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Projectile_Cone_01.fx
	End
End

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	32

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlash_Right
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		BhvrOverride
			TrackRate		10.0
		End
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	Prime
	End
End

#############################################################

End