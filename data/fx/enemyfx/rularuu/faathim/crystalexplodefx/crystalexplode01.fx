#########################################################
##	template

FxInfo

Lifespan 250
#########################################################
####################### AUDIO ###########################
#########################################################





#########################################################
#################### BODY GLOW 1 ########################
#########################################################
Condition
	On	Time
	Time	0
################## CRYSTAL ##########################
	
	Event	
		ENAME	ShrinkNode
		Type	Start
		At	Root
		Bhvr	:ShardScale.bhvr	
	End

	Event	
		ENAME	ShardNode
		Type	Start
		At	ShrinkNode
		Bhvr	:ShardOffset01.bhvr	
	End

	Event	
		ENAME	ShardNodeTop
		Type	Start
		At	ShrinkNode
		Bhvr	:ShardOffsetTop01.bhvr	
	End

End

Condition
	On	Time
	Time	7


################## X-LARGE SHARDS ##########################
	
	Event	
		ENAME	Shard01
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Bhvr	:ShardXLarge01.bhvr
		Lifespan 60
	End

	Event	
		ENAME	Shard02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard02
		Bhvr	:ShardXLarge01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Shard02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardXLarge01.bhvr
		Lifespan 60
	
	End


################## LARGE SHARDS ##########################
	Event	
		ENAME	Shard01
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardLarge01.bhvr
		Lifespan 60
	End

	Event	
		ENAME	Shard02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardLarge01.bhvr
		Lifespan 60
	
	End

################## SMALL SHARDS ##########################

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNode
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

################## SMALL SHARDS TOP ##########################

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall01
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall01.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ShardSmall02
		Type	Start
		At	ShardNodeTop
		#part	:CrystalExplode01.part
		#Geom	Fx_Iceweapon
		Geom	Fthm_Crystal_Chamber_Shard01
		Geom	Fthm_Crystal_Chamber_Shard02
		Geom	Fthm_Crystal_Chamber_Shard03
		Bhvr	:ShardSmall02.bhvr
		Lifespan 60
	
	End
End
################## PARTICLES ###############################
Condition
	On	Time
	Time	0
	

	Event	
		Type	Start
		At	Root
		part	:CrystalExplode01.part
		part	:CrystalExplode02.part
		Lifespan 10
	End

End

Condition
	On	Time
	Time	3

	Event	
		Type	Start
		At	Root
		part	:CrystalExplodeDustSparkles.part
		Lifespan 250
	End

End

Condition
	On	Time
	Time	3

	Event	
		Type	Start
		At	Root
		part	:CrystalExplodeDust01.part
		part	:CrystalExplodeDust01sml.part
		Lifespan 150
	End

End

Condition
	On	Time
	Time	30

	Event	
		Type	Start
		At	ShrinkNode
		part	:CrystalExplodeDust02.part
		Lifespan 70
	End


End

End