#############################################################
## SporeSleepBomb.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound grtoss2 70 70 0.6
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	WepR
		bhvr	CustomizeablePowers\PlantControl\PuffBallLobbprojectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\PlantControl\SporeDustProjectile.part
		Part	CustomizeablePowers\PlantControl\SporeDustProjectileMist.part
		Part	CustomizeablePowers\PlantControl\SporeDustGlows.part
		Geom	Tintable_PuffBall02
		Magnet	T_Chest
		LookAt	T_Chest
		Sound mushroomtoss 60 60 0.7
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	start
		At	T_Root
		Part	CustomizeablePowers\PlantControl\SporeDustScatter.part
		Part	CustomizeablePowers\PlantControl\SporeDustGlowsScatter.part
		Sound Mushroombombhit 80 80 0.88
		LifeSpan  30
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End