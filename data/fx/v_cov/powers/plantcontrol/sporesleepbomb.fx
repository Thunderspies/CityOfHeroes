#############################################################
## SporeSleepBomb.fx
#############################################################

FxInfo

LifeSpan  200

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound grtoss2 70 70 .6
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	WepR
		bhvr	:PuffBallLobbprojectile.bhvr
		Geom	Puffball02
		Part	:SporeDustProjectile.part
		Part	:SporeDustProjectileMist.part
		Part	:SporeDustGlows.part
		Magnet	T_Chest
		LookAt	T_Chest
		Sound mushroomtoss 60 60 .7
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	start
		At	T_Root
		Part	:SporeDustScatter.part
		Part	:SporeDustGlowsScatter.part
		Sound Mushroombombhit 80 80 .88
		LifeSpan  30
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End