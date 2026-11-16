#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan	90

########################################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MA_MasterfulThrow_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName	PrimeL
		Type	Local
		At	WepL
		Geom	GEO_WepR_Shuriken
		BhvrOverride
			Positionoffset		-.4 0 0
			Scale			2 2 2
			Spin			0 0 0
		End
	End

	Event
		EName	PrimeR
		Type	Local
		At	WepR
		Geom	GEO_WepR_Shuriken
		BhvrOverride
			Positionoffset		.4 0 0
			Scale			2 2 2
			Spin			0 0 0
		End
	End

####################################################################

	Event
		At	PrimeL
		Type	Local
		Ename	WeaponOffset
		Part	:Rings_Plasma.part
		PMagnet	WepL
		#Lifespan 45
	End

	Event
		At	PrimeR
		Type	Local
		Ename	WeaponOffset2
		Part	:Rings_Plasma.part
		PMagnet	WepR
		#Lifespan 45
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WeaponOffset
		Part	:Muzzle_Glow.part
		Part	:Muzzle_Glow_Core.part
		#Lifespan 90

	End

	Event
		Type	Local
		At	WeaponOffset2
		Part	:Muzzle_Glow.part
		Part	:Muzzle_Glow_Core.part
		#Lifespan 120
	End

End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs_Subtle.part
		PMagnet	WepL
		#Lifespan 30
	End

	Event
		Type	Local
		At	WeaponOffset2
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs_Subtle.part
		PMagnet	WepR
		#Lifespan 30
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs.part
		PMagnet	WepL
		#Lifespan 30
	End

	Event
		Type	Local
		At	WeaponOffset2
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs.part
		PMagnet	WepR
		#Lifespan 30
	End
End

#####################################

End

