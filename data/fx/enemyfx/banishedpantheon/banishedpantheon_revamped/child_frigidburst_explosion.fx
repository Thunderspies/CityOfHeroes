#############################################################
## BitterFreezeRay.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPB_FrigidSlash_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	Prime2
		Type	StartPositOnly
		At	Chest
		Part	:FrigidBurst_Explode_Particles.part
		Part	:Sentinel_ConsumingBlast_Shards.part
		Part	:Sentinel_ConsumingBlast_Smoke.part


		Lifespan 5
	End

End

#############################################################

End