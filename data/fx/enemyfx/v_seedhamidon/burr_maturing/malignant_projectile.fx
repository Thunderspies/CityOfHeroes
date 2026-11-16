#############################################################
## Fault.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Malignant_Projectile_01 100 100 .8
	End

	Event
		EName 	Prime
		Type	Start
		At	Chest
		Bhvr	behaviors\Braz\Projectile_Fast_01.bhvr
		BhvrOverride
			Spin			0 0.8 0

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		Part	:Malignant_Projectile_Add.part
		Part	:Malignant_Projectile_Alpha.part
		Magnet	Target
		LookAt	Target
		Lifespan 30
	End
End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Type	Local
		At	T_Chest
		Part	:Malignant_HitSplatter_Mult.part
		Part	:Malignant_Splatter_Spray.part
		PArt	:Malignant_HitFlash.part
		Lifespan 5
	End
End

#### Coninuing
##########################################################
#
#
#Condition
#	On 	PrimeHit
#	#Time	0
#
#	Event
#		Type	PositOnly
#		At	T_Chest
#		Bhvr 	Events\EndGame\KeyesIsland\ForceBubble_Disintegration.bhvr
#		BhvrOverride
#			Alpha		255
#			StartColor	100 255 10
#			PyrRotateJitter	-180.0 -180.0 0.0
#			FadeInLength	20
#			FadeOutLength	20
#			Spin 		10 10 0
#		End
#		Part	:Malignant_Globs.part
#		Part	:Malignant_Border.part
#		#Geom	Disintegration_Bubble
#	End
#End
#
########################
#
#Condition
#	On 	PrimeHit
#	#Time 	60
#
#	Event
#		EName   RadiusFxScale
#		Type	local
#		At	T_Hips
#		ChildFX	:Malignant_Continuing_CHILD.fx
#	End
#End

#############################################################

End