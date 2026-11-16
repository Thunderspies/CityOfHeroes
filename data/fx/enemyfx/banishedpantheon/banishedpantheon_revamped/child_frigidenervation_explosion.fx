#############################################################
## BitterFreezeRay.fx
#############################################################

FxInfo
Lifespan 100

#############################################################


Condition
	On	Time
	Time	0

	Event
		Ename	Prime2
		Type	StartPositOnly
		At	Chest
		Part	:Hit_Flash_Large.part
		Part	:FrigidBurst_Explode_Particles_Large.part
		Part	:Sentinel_ConsumingBlast_Shards.part
		Part	:Sentinel_ConsumingBlast_Smoke.part
		Part	:Hit_Shockwave_Main.part
		Part	:Sentinel_ConsumingBlast_Smoke_Additive.part
		Lifespan 5
	End

End

#############################################################

End