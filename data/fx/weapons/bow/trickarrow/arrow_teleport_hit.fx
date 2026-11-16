#
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 0

		Event
			Ename Prime
			At Target
			Type Start
			LookAt Chest
			Magnet Chest
			Bhvr behaviors\Braz\Projectile_FastWithUp_01.bhvr
			BhvrOverride
				Gravity 1.5
				InitialVelocity 0 15 0
				TrackRate 10
			End
			#Geom Geo_FX_TestSphere
			Geom	aEMPArrow
		End

		Event
			EName 	point1
			Type	Local
			At	Prime
			altpiv	1
			Part	WEAPONS\Bow\TrickArrow\EMPArch1.part
			Part	WEAPONS\Bow\TrickArrow\EMPArch2.part
			Part	WEAPONS\Bow\TrickArrow\EMPglows.part
			Pmagnet	point2
		End

		Event
			EName 	point2
			Type	Local
			At	Prime
			altpiv	2
			Part	WEAPONS\Bow\TrickArrow\EMPArch1.part
			Part	WEAPONS\Bow\TrickArrow\EMPArch2.part
			Part	WEAPONS\Bow\TrickArrow\EMPglows.part
			Pmagnet	point1
		End

		Event
			EName 	point3
			Type	Local
			At	Prime
			altpiv	1
			ChildFX	WEAPONS\Bow\TrickArrow\EMPArrow_Flash.fx
		End

		Event
			EName 	point4
			Type	Local
			At	Prime
			altpiv	2
			ChildFX	WEAPONS\Bow\TrickArrow\EMPArrow_Flash.fx
		End
	End




#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	prime
		Type	Destroy
	End

	Event
		Ename 	point1
		Type	Destroy
	End

	Event
		Ename 	point2
		Type	Destroy
	End

	Event
		Ename 	point3
		Type	Destroy
	End

	Event
		Ename 	point4
		Type	Destroy
	End

	Event
		Type	PositOnly
		At	Chest
		ChildFx	WEAPONS\Bow\TrickArrow\EmpPulse.fx
		LifeSpan 80
	End

	Event
		Type 	Local
		At 	Root
		ChildFx POWERS\Teleportation\TeleportRecall.fx
		EName 	Teleport
	End
End

End
