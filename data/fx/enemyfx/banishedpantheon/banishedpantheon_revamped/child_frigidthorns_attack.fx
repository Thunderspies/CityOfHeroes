############################################################
##	Fireweapon
###########################################################

FxInfo
Lifespan	30

Flags	InheritAnimScale

#################################################################################

Condition
	On	Time
	Time	0

		Event
			EName	SpineRotation
			At 	Hips
			Type 	Start
			BhvrOverride
				PYRRotate	10.0 0.0 0.0
				PYRRotateJitter	10.0 180.0 0.0
			End
			LifeSpan 	20
		End

		Event
			At 	SpineRotation
			Type 	Local
			Bhvr 	:FrigidThorns_Projectile.bhvr
			Part	:FrigidQuills_Trail.part
			Part	:FrigidQuills_Trail_Subtractive.part
			Geom 	tintable_FX_IceBlast2
			LifeSpan 	13
		End


End

#################################################################################

End