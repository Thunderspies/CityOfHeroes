#########################################################
##	
##

FxInfo
LifeSpan  200


######## Random Static Node  ###########################

Condition
	On	Time
	Time	0

	Event
		Ename	StaticNode
		Type	Start  
		At	WepR
	End
End




######## Activation flash  ###########################

Condition
	On	Time
	Time	2

	Event
		Type	Start
		At	StaticNode
		part2	POWERS\RadiationControl\RadiationAuraGreenLocal.part
		part3	POWERS\RadiationControl\RadiationAuraYellowLocal.part
		part4	POWERS\RadiationControl\RadiationAuraWhiteLocal.part	
		PMagnet	Chest
		LifeSpan	20
	End
End



##################################
Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start 
		At	StaticNode
		#Geom	Puffball
		Geom	Puffball02
		bhvr	:PuffBallLobbprojectile.bhvr
		Part	:SporeDustProjectile.part
		Part	:SporeDustProjectileMist.part
		Part	:SporeDustGlows.part
		Magnet	T_Chest
		LookAt	T_Chest
		Sound mushroomtoss 60 60 .7

	End

	
End

##################################


Condition
	On 	PrimeHit
	
	Event
		Type	start 
		At	T_Root
		Part	:SporeDustScatter.part
		Part	:SporeDustGlowsScatter.part
		LifeSpan  30
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


End				