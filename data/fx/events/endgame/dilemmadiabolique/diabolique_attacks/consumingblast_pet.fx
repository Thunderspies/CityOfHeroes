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
		Sound 	DD_ConsumingBlast_Pet_01 100 100 .8
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
			StartColor	150 50 200
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

		Part	:ConsumingBlast_Flash.part
		Part	:ConsumingBlast_Flash_Core.part
		Part	:ConsumingBlast_Streaks.part
		Part	:ConsumingBlast_Shards.part
		Part	:ConsumingBlast_Shards_Black.part
		Part	:ConsumingBlast_Sparkles.part
		Part	:ConsumingBlast_Flash_Glow.part
		Lifespan 5
	End

	Event
		Ename	Prime2
		Type	PositOnly
		At	Chest
		BhvrOverride
			StartColor	255 255 255
		End
		Part	:ConsumingBlast_Smoke.part
		Lifespan 3
	End
End

#############################################################

End