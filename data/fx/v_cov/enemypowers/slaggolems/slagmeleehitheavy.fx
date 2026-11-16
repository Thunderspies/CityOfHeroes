#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Target
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	Start 
		At	Chest
		Part1	:SlagDust1.part
		Part2	:SlagDust2.part
		Part	:SlagDustLarge.part
		Part	:SlagChunkHitFlash.part
		Part	:SlagDustRocks2.part
		Part	:SlagDustRocks2.part
		Part3	:SlagRocks.part
		Part4	:SlagGolemMeleeImpact.part
		#Bhvr	Behaviors/CameraShake02Subtle.bhvr
		Sound boulderhit 90 90 .99

	End

	
	Event
		EName	debris
		Type	Start
		At	WepR
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	200
				
	End
	
			
	Event
		EName	debris
		Type	Start
		At	WepR
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	200
				
	End
	
				
	Event
		EName	debris
		Type	Start
		At	WepR
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	200
				
	End
	



End


End
