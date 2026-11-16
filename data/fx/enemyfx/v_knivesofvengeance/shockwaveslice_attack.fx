#########################################################
##	template

FxInfo

Lifespan 36


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Mystic
		Sound BlackKnights_NetherBlade_Projectile 100.0 100.0 0.8
	End

	Event
		Type	Start
		At	Mystic
		Geom	CrossSwoosh1
		bhvr	:DualBlade_CrossSwoosh1.bhvr
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetOffset
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset		0.0 4.0 0.0
		End
		##Geom	GEO_FX_TestSphere
	End

	Event
		EName	ProjectileStartOffset
		Type	Start
		At	Chest
		LookAt	TargetOffset
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName	Projectile1
		Type	Local
		At	ProjectileStartOffset
		part	:DualBlades_Projectile2.part
		part	:DualBlades_Projectile3.part
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Origin
		Sound clobberwhoosh 100.0 100.0 .78
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
	End

End

################################################################################
##Streaks
################################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	Prime
		Type	start
		At	FootR
		Bhvr	behaviors/Maceprojectile2.bhvr

		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		
		Part	:Embers.part
		Part	:MaceRaysSparks.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Prime1
		Type	start
		At	FootR
		Bhvr	behaviors/Maceprojectile2.bhvr
		
		Part	:MaceEnergyRingsFlat.part
		Part	:MaceEnergyRings3.part
		
		Magnet	T_Root
		LookAt	T_Root
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName	all
		Type	Destroy
	
	End	

End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy

	End

End
	
End	

	