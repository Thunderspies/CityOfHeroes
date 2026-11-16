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
		Sound 	DD_SentinalConsumingBlast_Pet_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0
	Repeat	20

	Event
		Ename	Prime2
		Type	Start
		At	WepL
		Bhvr	behaviors\CameraShake_Strong20.bhvr

		Lifespan 20
	End

	Event
		Ename	Prime2
		Type	Start
		At	WepL
		Bhvr	:IceChunk_Hit.bhvr
		BhvrOverride
			StartColor	20 150 200
		End
		geom	tintable_FX_IceWall_01
		geom	tintable_FX_IceWall_02
		geom	tintable_FX_IceWall_03
		geom	tintable_FX_IceWall_09
		geom	tintable_FX_IceWall_10
		geom	tintable_FX_IceWall_11
		geom	tintable_FX_IceWall_12
		Lifespan 20
	End

End

Condition
	On	Time
	Time	0

	Event
		Ename	Prime2
		Type	StartPositOnly
		At	Chest
		Bhvr	:IceChunk_Hit.bhvr
		BhvrOverride
			StartColor	255 255 255
		End

		Part	:Sentinel_ConsumingBlast_Flash.part
		Part	:Sentinel_ConsumingBlast_Flash_Core.part
		Part	:Sentinel_ConsumingBlast_Streaks.part
		Part	:Sentinel_ConsumingBlast_Shards.part
		Part	:Sentinel_ConsumingBlast_Shards_Black.part
		Part	:Sentinel_ConsumingBlast_Sparkles.part
		Part	:Sentinel_ConsumingBlast_Flash_Glow.part
		Lifespan 5
	End

	Event
		Ename	Prime2
		Type	PositOnly
		At	Chest
		BhvrOverride
			StartColor	255 255 255
		End
		Part	:Sentinel_ConsumingBlast_Smoke.part
		Lifespan 3
	End
End

#############################################################

End