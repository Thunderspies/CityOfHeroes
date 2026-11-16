#########################################################
##	
##

FxInfo

##################################


Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At	Chest

		BHVR	Behaviors/GenericParticleFade.Bhvr
		BhvrOverride
			PositionOffset	0 -.2 0
		End

		Part	:PlasmaFire.part
		Part	:LaserHitGlow1.part
		Part	:PlasmaBlastHead.part

	End
	
End

##################################


End				