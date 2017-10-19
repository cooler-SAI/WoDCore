DELETE FROM gameobject WHERE map=1158;

---------------------------------------------------------
-- GameObject Spawn (All phase)
---------------------------------------------------------
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(181143, 1158, 1906.078, 210.5226, 78.73335, 1.596973, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(181144, 1158, 1906.432, 211.2292, 78.72942, 4.46489, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(223034, 1158, 1846.293, 236.2344, 77.50846, 5.79466, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229498, 1158, 1797.72, 206.2066, 70.17173, 3.490667, 268435455, 65535, 255, 0) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229750, 1158, 1900.248, 208.2031, 77.0564, 2.670348, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229751, 1158, 1903.188, 207.1042, 77.03136, 2.775069, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229752, 1158, 1896.847, 207.4219, 77.00218, 0.392699, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229753, 1158, 1898.5, 205.3958, 77.02212, 0.7592169, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229755, 1158, 1847.504, 235.4844, 76.65571, 2.286379, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229756, 1158, 1845.016, 236.9809, 76.65052, 6.047568, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229757, 1158, 1847.099, 237.6181, 76.65558, 3.935722, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229758, 1158, 1843.564, 239.5295, 76.66069, 1.213002, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229759, 1158, 1844.254, 242.4722, 76.65673, 4.34587, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229760, 1158, 1845.599, 240.6198, 76.65056, 3.141593, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(229769, 1158, 1781.184, 178.3299, 71.86462, 2.225294, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(230298, 1158, 1915.212, 202.7656, 76.76147, 0.9162973, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(230865, 1158, 1905.141, 214.4688, 78.42735, 3.478189, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232156, 1158, 1901.446, 204.2674, 76.91859, 3.516838, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232224, 1158, 1850.255, 262.9972, 78.19712, 3.361254, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232225, 1158, 1845.826, 260.8477, 78.19712, 0.3101675, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232234, 1158, 1844.96, 264.4932, 78.19712, 0.3100902, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232235, 1158, 1848.789, 267.1859, 78.19712, 3.354324, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1965.363, 334.0365, 88.95493, 2.809834, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1843.601, 256.4479, 78.23629, 2.809834, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1843.59, 256.3698, 78.23044, 3.850808, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1740.752, 209.6146, 73.50011, 2.809834, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1965.352, 333.9601, 88.95556, 3.850808, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1871.05, 199.5712, 77.38889, 2.809834, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1740.741, 209.5382, 73.50868, 3.850808, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232249, 1158, 1871.04, 199.4948, 77.39774, 3.850808, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232250, 1158, 1870.811, 199.9045, 77.24889, 1.2816, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232251, 1158, 1965.497, 333.3542, 88.87424, 1.355358, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232252, 1158, 1740.95, 210.0972, 73.34547, 1.271273, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232397, 1158, 1889.443, 252.7309, 76.62231, 3.49939, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(232528, 1158, 1751.74, 211.8472, 73.25347, 4.379183, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(233141, 1158, 1773.938, 177.0955, 71.88084, 0, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(233141, 1158, 1973.08, 89.84375, 85.37319, 0, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(233141, 1158, 1892.957, 272.4531, 76.60803, 0, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(233141, 1158, 1922.194, 327.1024, 89.66704, 0, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(233162, 1158, 1730.434, 133.7135, 75.959, 2.268925, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(233163, 1158, 1895.679, 271.2552, 76.60886, 2.766348, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(236356, 1158, 1853.075, 258.0556, 78.40399, 4.956738, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(236821, 1158, 1906.365, 212.099, 78.68111, 3.971839, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(239178, 1158, 1847.422, 269.9931, 78.31422, 4.95749, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(239184, 1158, 1868.415, 265.0955, 77.17195, 4.233185, 268435455, 65535, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);

---------------------------------------------------------
-- GameObject Spawn (Phase 2)
---------------------------------------------------------
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `phaseMask`, `animprogress`, `state`) VALUES(230773, 1158, 1868.408, 252.2465, 77.62025, 0, 268435455, 2, 255, 1) ON DUPLICATE KEY UPDATE `id` = VALUES(`id`), `map` = VALUES(`map`), `position_x` = VALUES(`position_x`), `position_y` = VALUES(`position_y`), `position_z` = VALUES(`position_z`), `orientation` = VALUES(`orientation`), `spawnMask` = VALUES(`spawnMask`), `phaseMask` = VALUES(`phaseMask`), `animprogress` = VALUES(`animprogress`), `state` = VALUES(`state`);
