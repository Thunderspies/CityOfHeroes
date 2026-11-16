#############################################################
## BitterIceBolt.fx
#############################################################

FxInfo

LifeSpan 200

## HANDS ###########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	DD_FrigidBolt_Attack_01 300 100 .8
	End
End

Condition
	On	Time
	Time	10

		Event
			At WepR
			Type Local
			EName RightHandMist
			Part :FrigidLance_Hand_Glow.part
			Part :FrigidLance_Hand_Glow_Additive.part
			Part :FrigidLance_Hand_Snow.part
			Part :FrigidLance_Hand_Snow_Additive.part
			Part :FrigidLance_Hand_Core.part
			LifeSpan 37
		End

End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	:FrigidBolt_Projectile.bhvr
		BhvrOverride
			StartColor	50 10 80
		End
		Geom	FX_FrigidLance
		Part	:FrigidBolt_Core_Small.part

		Magnet	Target
		LookAt	Target
	End


	Event
		EName 	Trail
		Type	Local
		At	Prime
		Part	:FrigidBolt_Snow.part
		Part	:FrigidBlast_Snow_Additive.part

	End
End

## ON-HIT ############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End