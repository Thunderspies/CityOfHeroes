#############################################################
## BitterIceBolt.fx
#############################################################

FxInfo
LifeSpan 200

## HANDS ###########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_FrigidLance_Buildup_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	80
	Event
		Type	Local
		At	Root
		Sound 	DD_FrigidBlast_Attack_01 300 100 .8
	End
End

Condition
	On	Time
	Time	10


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	:FrigidLance_Hand_Glow.part
		Part	:FrigidLance_Hand_Glow_Additive.part
		Part	:FrigidLance_Hand_Snow.part
		Part	:FrigidLance_Hand_Snow_Additive.part
		Part	:FrigidLance_Hand_Core.part
		LifeSpan 95
	End
End

Condition
	On 	Time
	Time 	22

	Event
		Type	Local
		At	WepL
		LifeSpan 90
		Part	:FrigidLance_Hand_Glow.part
		Part	:FrigidLance_Hand_Glow_Additive.part
		Part	:FrigidLance_Hand_Snow.part
		Part	:FrigidLance_Hand_Snow_Additive.part
		Part	:FrigidLance_Hand_Core.part
		LifeSpan 85

	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	80

	Event
		EName 	Prime
		Type	Start
		At	Chest
		Bhvr	:FrigidLance_Projectile.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			StartColor	50 10 120
		End
		Geom	FX_FrigidLance
		Part	:FrigidLance_CrystalStreaks.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Trail
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:FrigidLance_Glow.part
		Part	:FrigidBlast_Glow_Additive.part
		Part	:FrigidLance_Snow.part
		Part	:FrigidLance_Snow_Additive.part
		Part	:FrigidLance_Core.part
	End

	Event
		EName 	Core
		Type	Local
		At	Prime
		AltPiv	1
		Part	:FrigidLance_Shockwaves.part

	End
End

## ON-HIT ############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Trail
		Type	Destroy
	End

	Event
		Ename	Core
		Type	Destroy
	End
End

#############################################################

End