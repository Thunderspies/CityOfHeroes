#############################################################
## HellfireAttack.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################

Condition
	On 	Time
	Time  	8
	Event
		Type	Local
		At	Origin
		Sound Demon_Cast_01 100 100 .8
		Sound Demon_Cast_02 100 100 .8
		Sound Demon_Cast_03 100 100 .8
	End

End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Part	:Enchant_Hands_Fire.part
		Part	:Enchant_Hands_Steam.part
		Lifespan 45
	End
End


Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors\powers\coldcontrol\iceblast.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		80
			PhysForcePowerJitter	15
		End

		Part	:Demonling_Hellfire_Core_Blast.part
		Part	:Demonling_Hellfire_Body_Blast.part
		Part	:Demonling_Hellfire_Tail.part
		Part	:Demonling_Hellfire_Smoke_Trail.part
		Part	:Demonling_Hellfire_SparkTrail.part
		Magnet	Target
		LookAt	Target

	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

End

Condition
	On	Time
	Time	100

	Event
		Ename	all
		Type	Destroy
	End

End

#############################################################

End


